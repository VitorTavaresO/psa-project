# Correções Sugeridas para Problemas de Segurança e Desempenho
## Rocket.Chat React Native

*Baseado nas análises do Copilot e SonarQube*

---

## 🔐 PROBLEMAS DE SEGURANÇA - PRIORIDADE ALTA

### 1. Hash de Senha sem Salt (`app/lib/methods/helpers/localAuthentication.ts`)

**Problema:** Uso de SHA-256 simples para hash de senha do passcode local sem salt, facilitando ataques de força bruta.

**Localização:** Linha 73 - `UserPreferences.setString(PASSCODE_KEY, sha256(passcode));`

**Correção Sugerida:**
```typescript
import { randomBytes } from 'react-native-simple-crypto';

export const changePasscode = async ({ force = false }: { force: boolean }): Promise<void> => {
	const passcode = await openChangePasscodeModal({ force });
	
	// Gerar salt único
	const salt = await randomBytes(32); // 32 bytes = 256 bits
	const saltB64 = bufferToB64(salt);
	
	// Combinar passcode com salt antes do hash
	const saltedPasscode = passcode + saltB64;
	const hashedPasscode = sha256(saltedPasscode);
	
	// Armazenar hash e salt separadamente
	UserPreferences.setString(PASSCODE_KEY, hashedPasscode);
	UserPreferences.setString(`${PASSCODE_KEY}_SALT`, saltB64);
};

// Função para verificar passcode deve ser atualizada também
export const verifyPasscode = (inputPasscode: string): boolean => {
	const storedHash = UserPreferences.getString(PASSCODE_KEY);
	const storedSalt = UserPreferences.getString(`${PASSCODE_KEY}_SALT`);
	
	if (!storedHash || !storedSalt) return false;
	
	const saltedInput = inputPasscode + storedSalt;
	const inputHash = sha256(saltedInput);
	
	return inputHash === storedHash;
};
```

**Justificativa:** Adicionar salt torna cada hash único, mesmo para senhas idênticas, dificultando ataques de dicionário e rainbow tables.

---

### 2. Chaves Privadas E2E em Texto Simples (`app/lib/encryption/encryption.ts`)

**Problema:** Chaves privadas E2E armazenadas em texto simples no UserPreferences após descriptografia.

**Localização:** Linha 167 - método `createKeys` e armazenamento de chaves

**Correção Sugerida:**
```typescript
// Adicionar criptografia adicional para chaves privadas
import { AES } from 'react-native-simple-crypto';

class Encryption {
	// Método para criptografar chave privada antes do armazenamento
	private encryptPrivateKeyForStorage = async (privateKey: string, userPassword: string): Promise<string> => {
		// Derivar chave de criptografia do password do usuário
		const keyDerivation = await SimpleCrypto.PBKDF2.hash(
			utf8ToBuffer(userPassword),
			utf8ToBuffer('rocket_chat_private_key_salt'),
			10000, // iterações
			32, // tamanho da chave
			'SHA256'
		);
		
		// Criptografar chave privada
		const encryptedKey = await AES.encrypt(privateKey, keyDerivation);
		return encryptedKey;
	};

	// Método para descriptografar chave privada do armazenamento
	private decryptPrivateKeyFromStorage = async (encryptedKey: string, userPassword: string): Promise<string> => {
		const keyDerivation = await SimpleCrypto.PBKDF2.hash(
			utf8ToBuffer(userPassword),
			utf8ToBuffer('rocket_chat_private_key_salt'),
			10000,
			32,
			'SHA256'
		);
		
		const decryptedKey = await AES.decrypt(encryptedKey, keyDerivation);
		return decryptedKey;
	};

	// Atualizar método persistKeys
	persistKeys = (server: string, publicKey: any, privateKey: string, userPassword?: string) => {
		if (userPassword) {
			// Criptografar antes de armazenar
			const encryptedPrivateKey = await this.encryptPrivateKeyForStorage(privateKey, userPassword);
			UserPreferences.setString(`${server}-${E2E_PRIVATE_KEY}`, encryptedPrivateKey);
		} else {
			// Log de alerta - chave sendo armazenada sem criptografia adicional
			console.warn('Private key stored without additional encryption');
			UserPreferences.setString(`${server}-${E2E_PRIVATE_KEY}`, privateKey);
		}
		UserPreferences.setString(`${server}-${E2E_PUBLIC_KEY}`, publicKey);
	};
}
```

**Justificativa:** Adicionar uma camada extra de criptografia para chaves privadas protege contra acesso não autorizado ao armazenamento local.

---

### 3. Geração de Chaves com Entropia Insuficiente (`app/lib/encryption/room.ts`)

**Problema:** Geração de chaves de sala usando apenas 16 bytes aleatórios pode ser insuficiente para segurança a longo prazo.

**Localização:** Geração de chaves de sala

**Correção Sugerida:**
```typescript
// Aumentar tamanho da chave e adicionar validação de entropia
export const generateRoomKey = async (): Promise<ArrayBuffer> => {
	// Usar 32 bytes (256 bits) em vez de 16 bytes (128 bits)
	const keySize = 32;
	
	// Gerar bytes aleatórios com alta entropia
	const randomBytes = await SimpleCrypto.utils.randomBytes(keySize);
	
	// Validar qualidade da entropia (opcional, mas recomendado)
	if (!validateEntropyQuality(randomBytes)) {
		throw new Error('Insufficient entropy for key generation');
	}
	
	return randomBytes;
};

// Função para validar qualidade da entropia
const validateEntropyQuality = (bytes: ArrayBuffer): boolean => {
	const uint8Array = new Uint8Array(bytes);
	
	// Verificar se não há muitos bytes repetidos
	const uniqueBytes = new Set(uint8Array);
	const uniqueRatio = uniqueBytes.size / uint8Array.length;
	
	// Deve ter pelo menos 75% de bytes únicos
	return uniqueRatio >= 0.75;
};

// Atualizar geração de chaves AESCTR
export const generateAESCTRKey = async (): Promise<ArrayBuffer> => {
	// Usar 32 bytes para AES-256
	return await generateRoomKey();
};
```

**Justificativa:** Chaves de 256 bits oferecem melhor segurança a longo prazo e proteção contra avanços computacionais futuros.

---

### 4. Tokens Sem Verificação de Integridade (`app/sagas/login.js`)

**Problema:** Tokens de autenticação armazenados no Keychain iOS sem verificação de integridade.

**Correção Sugerida:**
```typescript
import { createHmac } from 'react-native-simple-crypto';

// Adicionar HMAC para verificação de integridade
const generateTokenHMAC = async (token: string, secret: string): Promise<string> => {
	const hmac = await createHmac(token, secret, 'SHA256');
	return hmac;
};

// Método para armazenar token com verificação de integridade
const storeTokenSecurely = async (server: string, token: string, userId: string) => {
	try {
		// Gerar secret único para este servidor/usuário
		const secret = await SimpleCrypto.utils.randomBytes(32);
		const secretB64 = bufferToB64(secret);
		
		// Gerar HMAC do token
		const tokenHMAC = await generateTokenHMAC(token, secretB64);
		
		// Armazenar token, HMAC e secret
		UserPreferences.setString(`${TOKEN_KEY}-${server}`, token);
		UserPreferences.setString(`${TOKEN_KEY}-${server}-HMAC`, tokenHMAC);
		UserPreferences.setString(`${TOKEN_KEY}-${server}-SECRET`, secretB64);
		
		if (isIOS) {
			await Keychain.setInternetCredentials(server, userId, token);
		}
	} catch (error) {
		console.error('Error storing token securely:', error);
		throw error;
	}
};

// Método para verificar integridade do token
const verifyTokenIntegrity = async (server: string): Promise<boolean> => {
	try {
		const token = UserPreferences.getString(`${TOKEN_KEY}-${server}`);
		const storedHMAC = UserPreferences.getString(`${TOKEN_KEY}-${server}-HMAC`);
		const secret = UserPreferences.getString(`${TOKEN_KEY}-${server}-SECRET`);
		
		if (!token || !storedHMAC || !secret) {
			return false;
		}
		
		const calculatedHMAC = await generateTokenHMAC(token, secret);
		return calculatedHMAC === storedHMAC;
	} catch (error) {
		console.error('Error verifying token integrity:', error);
		return false;
	}
};
```

**Justificativa:** HMAC permite detectar modificações não autorizadas nos tokens armazenados.

---

### 5. Credenciais Basic Auth em Base64 Simples (`app/views/NewServerView/methods/basicAuth.ts`)

**Problema:** Credenciais Basic Auth armazenadas em Base64 simples sem criptografia adicional.

**Localização:** `basicAuth.ts` linha 8

**Correção Sugerida:**
```typescript
import { AES } from 'react-native-simple-crypto';
import { sha256 } from 'js-sha256';

const basicAuth = async (server: string, text: string) => {
	try {
		const parsedUrl = parse(text, true);
		if (parsedUrl.auth.length) {
			// Em vez de apenas Base64, usar criptografia AES
			const credentials = parsedUrl.auth;
			
			// Gerar chave de criptografia baseada no servidor
			const serverKey = sha256(server + 'basic_auth_encryption_salt');
			
			// Criptografar credenciais
			const encryptedCredentials = await AES.encrypt(credentials, serverKey);
			
			// Armazenar credenciais criptografadas
			UserPreferences.setString(`${BASIC_AUTH_KEY}-${server}`, encryptedCredentials);
			
			// Para uso imediato, ainda configurar Basic Auth
			const base64Credentials = Base64.encode(credentials);
			setBasicAuth(base64Credentials);
		}
	} catch (error) {
		console.error('Error storing basic auth credentials:', error);
	}
};

// Função para recuperar credenciais descriptografadas
const getBasicAuthCredentials = async (server: string): Promise<string | null> => {
	try {
		const encryptedCredentials = UserPreferences.getString(`${BASIC_AUTH_KEY}-${server}`);
		if (!encryptedCredentials) return null;
		
		const serverKey = sha256(server + 'basic_auth_encryption_salt');
		const decryptedCredentials = await AES.decrypt(encryptedCredentials, serverKey);
		
		return Base64.encode(decryptedCredentials);
	} catch (error) {
		console.error('Error retrieving basic auth credentials:', error);
		return null;
	}
};
```

**Justificativa:** Base64 não é criptografia e pode ser facilmente decodificado. AES oferece proteção real.

---

### 6. Validação de Certificados SSL/TLS (`app/lib/methods/helpers/sslPinning.ts`)

**Problema:** Senhas de certificados solicitadas via Alert.prompt sem validação de força.

**Localização:** Linha 33-49

**Correção Sugerida:**
```typescript
// Função para validar força da senha do certificado
const validateCertificatePassword = (password: string): { valid: boolean; message?: string } => {
	if (!password || password.length < 8) {
		return { valid: false, message: 'Password must be at least 8 characters long' };
	}
	
	if (!/[A-Z]/.test(password)) {
		return { valid: false, message: 'Password must contain at least one uppercase letter' };
	}
	
	if (!/[a-z]/.test(password)) {
		return { valid: false, message: 'Password must contain at least one lowercase letter' };
	}
	
	if (!/[0-9]/.test(password)) {
		return { valid: false, message: 'Password must contain at least one number' };
	}
	
	if (!/[!@#$%^&*()_+\-=\[\]{};':"\\|,.<>\/?]/.test(password)) {
		return { valid: false, message: 'Password must contain at least one special character' };
	}
	
	return { valid: true };
};

// Função melhorada para solicitar senha do certificado
const promptCertificatePassword = (): Promise<string> => {
	return new Promise((resolve, reject) => {
		const showPasswordPrompt = (errorMessage?: string) => {
			Alert.prompt(
				I18n.t('Certificate_password'),
				errorMessage || I18n.t('Whats_the_password_for_your_certificate'),
				[
					{
						text: I18n.t('Cancel'),
						onPress: () => reject(new Error('Password prompt cancelled')),
						style: 'cancel'
					},
					{
						text: 'OK',
						onPress: async (password) => {
							if (!password) {
								showPasswordPrompt('Password cannot be empty');
								return;
							}
							
							const validation = validateCertificatePassword(password);
							if (!validation.valid) {
								showPasswordPrompt(validation.message);
								return;
							}
							
							resolve(password);
						}
					}
				],
				'secure-text'
			);
		};
		
		showPasswordPrompt();
	});
};

// Atualizar método pickCertificate
ios: {
	pickCertificate: () =>
		new Promise(async (resolve, reject) => {
			try {
				const res = await DocumentPicker.getDocumentAsync({
					type: 'application/x-pkcs12'
				});
				if (res.canceled) {
					return reject(new Error('Certificate selection cancelled'));
				}
				
				const { uri, name } = res.assets[0];
				
				try {
					const password = await promptCertificatePassword();
					const certificatePath = getPath(name);
					await FileSystem.copyAsync({ from: uri, to: certificatePath });
					persistCertificate(name, password);
					resolve(name);
				} catch (passwordError) {
					reject(passwordError);
				}
			} catch (e) {
				reject(e);
			}
		}),
	// ... resto do código
}
```

**Justificativa:** Senhas fracas para certificados podem comprometer toda a segurança da conexão SSL/TLS.

---

## ⚠️ PROBLEMAS DE SEGURANÇA - PRIORIDADE MÉDIA

### 7. Estados OAuth com Entropia Baixa (`app/containers/LoginServices/serviceLogin.ts`)

**Problema:** Estados OAuth gerados com apenas 43 caracteres podem ser previsíveis.

**Localização:** Linha 137 - `const credentialToken = random(43);`

**Correção Sugerida:**
```typescript
import { randomBytes } from 'react-native-simple-crypto';

// Função melhorada para gerar token de credencial
const generateSecureCredentialToken = async (): Promise<string> => {
	// Usar 64 bytes (512 bits) de entropia
	const randomBuffer = await randomBytes(64);
	
	// Converter para string segura (Base64 URL-safe)
	const token = bufferToB64URI(randomBuffer);
	
	// Adicionar timestamp para garantir unicidade
	const timestamp = Date.now().toString(36);
	
	return `${token}_${timestamp}`;
};

const getOAuthState = async (loginStyle: TLoginStyle = 'popup') => {
	// Usar geração segura de token
	const credentialToken = await generateSecureCredentialToken();
	
	let obj: {
		loginStyle: string;
		credentialToken: string;
		isCordova: boolean;
		redirectUrl?: string;
	} = { loginStyle, credentialToken, isCordova: true };
	
	if (loginStyle === 'redirect') {
		obj = {
			...obj,
			redirectUrl: 'rocketchat://auth'
		};
	}
	return Base64.encodeURI(JSON.stringify(obj));
};
```

**Justificativa:** Tokens OAuth previsíveis podem permitir ataques de CSRF e session hijacking.

---

### 8. Rate Limiting para Login (`app/lib/services/connect.ts`)

**Problema:** Múltiplas tentativas de login sem rate limiting implementado.

**Correção Sugerida:**
```typescript
// Sistema de rate limiting para tentativas de login
class LoginRateLimiter {
	private attempts: Map<string, { count: number; lastAttempt: number; blocked: boolean }> = new Map();
	private readonly maxAttempts = 5;
	private readonly blockDuration = 300000; // 5 minutos
	private readonly attemptWindow = 900000; // 15 minutos

	isBlocked(identifier: string): boolean {
		const record = this.attempts.get(identifier);
		if (!record) return false;

		// Se ainda estiver no período de bloqueio
		if (record.blocked && (Date.now() - record.lastAttempt) < this.blockDuration) {
			return true;
		}

		// Reset se passou do período de bloqueio
		if (record.blocked && (Date.now() - record.lastAttempt) >= this.blockDuration) {
			this.attempts.delete(identifier);
			return false;
		}

		return false;
	}

	recordAttempt(identifier: string, success: boolean): void {
		if (success) {
			// Reset on successful login
			this.attempts.delete(identifier);
			return;
		}

		const now = Date.now();
		const record = this.attempts.get(identifier) || { count: 0, lastAttempt: 0, blocked: false };

		// Reset counter if outside attempt window
		if ((now - record.lastAttempt) > this.attemptWindow) {
			record.count = 0;
		}

		record.count++;
		record.lastAttempt = now;

		// Block if exceeded max attempts
		if (record.count >= this.maxAttempts) {
			record.blocked = true;
		}

		this.attempts.set(identifier, record);
	}

	getTimeUntilUnblock(identifier: string): number {
		const record = this.attempts.get(identifier);
		if (!record || !record.blocked) return 0;

		const timeLeft = this.blockDuration - (Date.now() - record.lastAttempt);
		return Math.max(0, timeLeft);
	}
}

const loginLimiter = new LoginRateLimiter();

async function login(credentials: ICredentials, isFromWebView = false): Promise<ILoggedUser | undefined> {
	const identifier = credentials.user || credentials.username || 'unknown';
	
	// Verificar se está bloqueado
	if (loginLimiter.isBlocked(identifier)) {
		const timeLeft = loginLimiter.getTimeUntilUnblock(identifier);
		throw new Error(`Too many login attempts. Try again in ${Math.ceil(timeLeft / 60000)} minutes.`);
	}

	try {
		// RC 0.64.0
		await sdk.current.login(credentials);
		
		// Registrar sucesso
		loginLimiter.recordAttempt(identifier, true);
		
		// ... resto da lógica de login
	} catch (error) {
		// Registrar falha
		loginLimiter.recordAttempt(identifier, false);
		throw error;
	}
}
```

**Justificativa:** Rate limiting previne ataques de força bruta contra credenciais de usuário.

---

## 🚀 PROBLEMAS DE DESEMPENHO

### 9. Operações de Criptografia Bloqueantes (`app/lib/encryption/utils.ts`)

**Problema:** Operações de criptografia/descriptografia sem paralelização.

**Correção Sugerida:**
```typescript
// Worker pool para operações de criptografia
class CryptoWorkerPool {
	private queue: Array<() => Promise<any>> = [];
	private running = 0;
	private readonly maxConcurrent = 3;

	async execute<T>(operation: () => Promise<T>): Promise<T> {
		return new Promise((resolve, reject) => {
			this.queue.push(async () => {
				try {
					const result = await operation();
					resolve(result);
				} catch (error) {
					reject(error);
				}
			});
			this.processQueue();
		});
	}

	private async processQueue(): Promise<void> {
		if (this.running >= this.maxConcurrent || this.queue.length === 0) {
			return;
		}

		this.running++;
		const operation = this.queue.shift()!;
		
		try {
			await operation();
		} finally {
			this.running--;
			this.processQueue();
		}
	}
}

const cryptoPool = new CryptoWorkerPool();

// Funções de criptografia não-bloqueantes
export const encryptMessageAsync = async (message: string, key: ArrayBuffer): Promise<string> => {
	return cryptoPool.execute(async () => {
		return await encryptAESCTR(message, key);
	});
};

export const decryptMessageAsync = async (encryptedMessage: string, key: ArrayBuffer): Promise<string> => {
	return cryptoPool.execute(async () => {
		return await decryptAESCTR(encryptedMessage, key);
	});
};

// Processamento em lote para múltiplas mensagens
export const encryptMultipleMessages = async (
	messages: Array<{ id: string; content: string }>,
	key: ArrayBuffer
): Promise<Array<{ id: string; encrypted: string }>> => {
	const promises = messages.map(msg => 
		cryptoPool.execute(async () => ({
			id: msg.id,
			encrypted: await encryptAESCTR(msg.content, key)
		}))
	);
	
	return Promise.all(promises);
};
```

**Justificativa:** Paralelização e pool de workers evitam bloqueios da UI durante operações criptográficas pesadas.

---

### 10. Queries de Banco Não Otimizadas (`app/lib/database/services/Subscription.ts`)

**Problema:** Queries de banco não otimizadas para dados de criptografia.

**Correção Sugerida:**
```typescript
// Cache para dados de criptografia frequentemente acessados
class EncryptionCache {
	private cache = new Map<string, { data: any; timestamp: number; ttl: number }>();
	private readonly defaultTTL = 300000; // 5 minutos

	set(key: string, data: any, ttl: number = this.defaultTTL): void {
		this.cache.set(key, {
			data,
			timestamp: Date.now(),
			ttl
		});
	}

	get(key: string): any | null {
		const entry = this.cache.get(key);
		if (!entry) return null;

		if (Date.now() - entry.timestamp > entry.ttl) {
			this.cache.delete(key);
			return null;
		}

		return entry.data;
	}

	invalidate(pattern: string): void {
		const regex = new RegExp(pattern);
		for (const [key] of this.cache) {
			if (regex.test(key)) {
				this.cache.delete(key);
			}
		}
	}

	clear(): void {
		this.cache.clear();
	}
}

const encryptionCache = new EncryptionCache();

// Queries otimizadas com cache
export const getSubscriptionByRoomIdOptimized = async (roomId: string): Promise<TSubscriptionModel | null> => {
	const cacheKey = `subscription_${roomId}`;
	
	// Verificar cache primeiro
	let subscription = encryptionCache.get(cacheKey);
	if (subscription) {
		return subscription;
	}

	const db = database.active;
	const subscriptionsCollection = db.get('subscriptions');
	
	try {
		// Query otimizada com índices específicos para criptografia
		subscription = await subscriptionsCollection
			.query(
				Q.where('rid', roomId),
				Q.select('id', 'rid', 'E2EKey', 'e2eKeyId', 'E2ESuggestedKey', 'encrypted')
			)
			.fetch();
			
		if (subscription.length > 0) {
			const result = subscription[0];
			// Cache apenas se contém dados de criptografia
			if (result.E2EKey || result.e2eKeyId || result.E2ESuggestedKey) {
				encryptionCache.set(cacheKey, result, 300000); // 5 min TTL
			}
			return result;
		}
	} catch (error) {
		console.error('Error fetching subscription:', error);
	}
	
	return null;
};

// Busca em lote para múltiplas salas
export const getMultipleSubscriptionsOptimized = async (roomIds: string[]): Promise<TSubscriptionModel[]> => {
	const cacheKey = `subscriptions_${roomIds.sort().join('_')}`;
	
	let subscriptions = encryptionCache.get(cacheKey);
	if (subscriptions) {
		return subscriptions;
	}

	const db = database.active;
	const subscriptionsCollection = db.get('subscriptions');
	
	try {
		subscriptions = await subscriptionsCollection
			.query(
				Q.where('rid', Q.oneOf(roomIds)),
				Q.select('id', 'rid', 'E2EKey', 'e2eKeyId', 'E2ESuggestedKey', 'encrypted')
			)
			.fetch();
			
		// Cache resultado por tempo menor para dados múltiplos
		encryptionCache.set(cacheKey, subscriptions, 180000); // 3 min TTL
		return subscriptions;
	} catch (error) {
		console.error('Error fetching multiple subscriptions:', error);
		return [];
	}
};
```

**Justificativa:** Cache reduz queries desnecessárias ao banco e queries específicas melhoram performance.

---

## 🐛 CORREÇÕES DE BUGS (SonarQube)

### 11. Bug de Regex (`app/containers/message/Emoji.tsx`)

**Problema:** Bug relacionado à regex identificado pelo SonarQube.

**Correção Sugerida:**
```typescript
// Regex segura e otimizada para emojis
const createEmojiRegex = (): RegExp => {
	// Usar regex escape para caracteres especiais
	const escapeRegex = (str: string): string => {
		return str.replace(/[.*+?^${}()|[\]\\]/g, '\\$&');
	};
	
	// Definir padrões de emoji de forma mais segura
	const emojiPattern = [
		':[a-zA-Z0-9_+-]+:', // Emoji shortcodes
		'[\u{1F600}-\u{1F64F}]', // Emoticons
		'[\u{1F300}-\u{1F5FF}]', // Misc Symbols
		'[\u{1F680}-\u{1F6FF}]', // Transport
		'[\u{2600}-\u{26FF}]', // Misc symbols
		'[\u{2700}-\u{27BF}]'  // Dingbats
	].join('|');
	
	return new RegExp(`(${emojiPattern})`, 'gu');
};

// Uso seguro da regex
const processEmojiText = (text: string): string => {
	try {
		const emojiRegex = createEmojiRegex();
		return text.replace(emojiRegex, (match) => {
			// Processar emoji de forma segura
			return processEmojiMatch(match);
		});
	} catch (error) {
		console.error('Error processing emoji text:', error);
		return text; // Retornar texto original em caso de erro
	}
};
```

**Justificativa:** Regex malformadas podem causar ReDoS (Regular Expression Denial of Service) ou comportamentos inesperados.

---

### 12. Problemas de Atribuição de Estado (`app/views/NotificationPreferencesView/index.tsx`)

**Problema:** Atribuição da mesma variável de estado no próprio setter (`setSwitchValue(switchValue)`).

**Correção Sugerida:**
```typescript
// Corrigir setters de estado que não fazem nada
const [switchValue, setSwitchValue] = useState(false);

// Em vez de:
// setSwitchValue(switchValue); // Bug: não faz nada

// Usar:
const handleSwitchChange = useCallback((newValue: boolean) => {
	setSwitchValue(newValue);
	// Lógica adicional se necessário
	onSwitchValueChange?.(newValue);
}, [onSwitchValueChange]);

// Para reset/toggle:
const resetSwitch = useCallback(() => {
	setSwitchValue(false); // ou valor padrão
}, []);

const toggleSwitch = useCallback(() => {
	setSwitchValue(prev => !prev);
}, []);
```

**Justificativa:** Setters que não alteram o estado são inúteis e podem indicar lógica incorreta.

---

### 13. Uso Correto de Await (`app/sagas/rooms.js`)

**Problema:** Uso incorreto ou falta de `await` em operações assíncronas.

**Correção Sugerida:**
```typescript
// Exemplo de correção para saga
function* handleRoomOperation(action) {
	try {
		// Certificar-se de que operações assíncronas usam yield para sagas
		const result = yield call(apiOperation, action.payload);
		
		// Para operações que retornam Promise, usar yield
		yield put(successAction(result));
		
		// Para múltiplas operações paralelas
		const [result1, result2] = yield all([
			call(operation1),
			call(operation2)
		]);
		
	} catch (error) {
		yield put(errorAction(error));
	}
}

// Para funções async regulares (não sagas)
const handleAsyncOperation = async (data) => {
	try {
		// Sempre aguardar operações assíncronas
		const result = await performOperation(data);
		
		// Para operações paralelas
		const [result1, result2] = await Promise.all([
			operation1(data),
			operation2(data)
		]);
		
		return result;
	} catch (error) {
		console.error('Async operation failed:', error);
		throw error;
	}
};
```

**Justificativa:** Uso incorreto de async/await pode causar race conditions e comportamentos inesperados.

---

## 📋 IMPLEMENTAÇÃO RECOMENDADA

### Ordem de Prioridade:
1. **Imediata:** Problemas de segurança alta prioridade (1-6)
2. **Próxima Sprint:** Problemas de segurança média prioridade (7-8)
3. **Melhorias Graduais:** Problemas de desempenho (9-10)
4. **Manutenção Contínua:** Correções de bugs (11-13)

### Testes Necessários:
- Testes unitários para todas as funções de criptografia
- Testes de integração para autenticação
- Testes de performance para operações paralelas
- Testes de segurança para validação de senhas

### Monitoramento:
- Logs de tentativas de login falhadas
- Métricas de performance de criptografia
- Alertas para falhas de integridade de tokens
- Monitoramento de uso de cache

---

*Documento gerado em: 28 de julho de 2025*  
*Base: Análises Copilot e SonarQube do Rocket.Chat React Native*
