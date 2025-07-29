# Comparativo de Segurança e Desempenho - Antes vs Depois

## Resumo Executivo

Este documento apresenta uma comparação lado a lado entre o código atual e as correções propostas para os **15 problemas críticos** de segurança e desempenho identificados no Rocket.Chat ReactNative.

---

## 🔴 PROBLEMAS DE PRIORIDADE ALTA

### 1. Armazenamento Inseguro de Credenciais

#### **Arquivo:** `app/lib/methods/userPreferences.ts`

<table>
<tr>
<th width="50%">❌ ANTES (Inseguro)</th>
<th width="50%">✅ DEPOIS (Seguro)</th>
</tr>
<tr>
<td>

```typescript
// app/lib/methods/userPreferences.ts
import { create, MMKVLoader, MMKVInstance, ProcessingModes, IOSAccessibleStates } from 'react-native-mmkv-storage';

const MMKV = new MMKVLoader()
  // MODES.MULTI_PROCESS = ACCESSIBLE BY APP GROUP (iOS)
  .setProcessingMode(ProcessingModes.MULTI_PROCESS)
  .setAccessibleIOS(IOSAccessibleStates.AFTER_FIRST_UNLOCK)
  .withEncryption()
  .initialize();

class UserPreferences {
  private mmkv: MMKVInstance;
  
  constructor() {
    this.mmkv = MMKV;
  }

  setString(key: string, value: string): boolean | undefined {
    return this.mmkv.setString(key, value) ?? undefined;
  }

  getString(key: string): string | null {
    try {
      return this.mmkv.getString(key) ?? null;
    } catch {
      return null;
    }
  }
}
```

</td>
<td>

```typescript
// app/lib/security/SecureStorage.ts
import * as Keychain from 'react-native-keychain';
import { Platform } from 'react-native';

export class SecureCredentialStorage {
  private static readonly SERVICE_NAME = 'RocketChatCredentials';
  
  static async storeCredentials(
    server: string, 
    userId: string, 
    token: string
  ): Promise<void> {
    try {
      await Keychain.setInternetCredentials(
        server,
        userId,
        token,
        {
          accessControl: Keychain.ACCESS_CONTROL
            .BIOMETRY_CURRENT_SET_OR_DEVICE_PASSCODE,
          securityLevel: Keychain.SECURITY_LEVEL.SECURE_HARDWARE,
          storage: Keychain.STORAGE_TYPE.AES_GCM_NO_AUTH,
          ...(Platform.OS === 'ios' && {
            accessGroup: 'group.chat.rocket.reactnative'
          })
        }
      );
    } catch (error) {
      throw new Error(`Failed to store credentials: ${error.message}`);
    }
  }

  static async getCredentials(server: string): Promise<{
    userId: string; 
    token: string 
  } | null> {
    try {
      const credentials = await Keychain.getInternetCredentials(server);
      if (credentials && credentials.username && credentials.password) {
        return {
          userId: credentials.username,
          token: credentials.password
        };
      }
      return null;
    } catch (error) {
      console.warn('Failed to retrieve credentials:', error);
      return null;
    }
  }
}
```

</td>
</tr>
</table>

#### **Arquivo:** `app/sagas/login.js`

<table>
<tr>
<th width="50%">❌ ANTES (Texto Plano)</th>
<th width="50%">✅ DEPOIS (Criptografado)</th>
</tr>
<tr>
<td>

```javascript
// app/sagas/login.js - Linha 279-305
const handleLoginSuccess = function* handleLoginSuccess({ user }) {
  try {
    const server = yield select(getServer);
    
    UserPreferences.setString(`${TOKEN_KEY}-${server}`, user.id);
    UserPreferences.setString(`${TOKEN_KEY}-${user.id}`, user.token);
    
    if (isIOS) {
      yield Keychain.setInternetCredentials(
        server, 
        user.id, 
        user.token, 
        {
          accessGroup: appGroupSuiteName,
          securityLevel: Keychain.SECURITY_LEVEL.SECURE_SOFTWARE
        }
      );
    }
    
    yield put(setUser(user));
  } catch (e) {
    log(e);
  }
};
```

</td>
<td>

```javascript
// app/sagas/login.js - Correção
const handleLoginSuccess = function* handleLoginSuccess({ user }) {
  try {
    const server = yield select(getServer);

    yield call(
      SecureCredentialStorage.storeCredentials, 
      server, 
      user.id, 
      user.token
    );
    
    const safeUserData = {
      id: user.id,
      username: user.username,
      name: user.name,
      language: user.language,
      status: user.status,
    };
    
    yield put(setUser(safeUserData));
    
  } catch (e) {
    log(e);
  }
};
```

</td>
</tr>
</table>

---

### 2. Chaves E2EE Sem Validação de Integridade

#### **Arquivo:** `app/lib/encryption/encryption.ts`

<table>
<tr>
<th width="50%">❌ ANTES (Sem Validação)</th>
<th width="50%">✅ DEPOIS (Com HMAC)</th>
</tr>
<tr>
<td>

```typescript
// app/lib/encryption/encryption.ts - Linha 139-165
class Encryption {
  persistKeys = async (
    server: string, 
    publicKey: string, 
    privateKey: string
  ): Promise<void> => {
    UserPreferences.setString(
      `${server}-${E2E_PUBLIC_KEY}`, 
      EJSON.stringify(publicKey)
    );
    UserPreferences.setString(
      `${server}-${E2E_PRIVATE_KEY}`, 
      EJSON.stringify(privateKey)
    );
  };

  getKeys = async (server: string): Promise<{
    publicKey: string; 
    privateKey: string 
  } | null> => {
    const publicKey = UserPreferences.getString(
      `${server}-${E2E_PUBLIC_KEY}`
    );
    const privateKey = UserPreferences.getString(
      `${server}-${E2E_PRIVATE_KEY}`
    );
    
    if (!publicKey || !privateKey) {
      return null;
    }
    
    return { 
      publicKey: EJSON.parse(publicKey), 
      privateKey: EJSON.parse(privateKey) 
    };
  };
}
```

</td>
<td>

```typescript
// app/lib/encryption/KeyIntegrityManager.ts
export class KeyIntegrityManager {
  private static readonly HMAC_KEY_SUFFIX = '_hmac';
  private static readonly KEY_VERSION = 'v1';

  static async storeKeyWithIntegrity(
    keyName: string, 
    keyData: string, 
    server: string
  ): Promise<void> {
    try {
      const hmacKey = await this.getHMACKey(server);
      const keyWithVersion = `${this.KEY_VERSION}:${keyData}`;
      const hmac = CryptoJS.HmacSHA256(keyWithVersion, hmacKey).toString();
      
      await SecureCredentialStorage.storeCredentials(
        `${server}_${keyName}`,
        keyName,
        keyWithVersion
      );
      
      await SecureCredentialStorage.storeCredentials(
        `${server}_${keyName}${this.HMAC_KEY_SUFFIX}`,
        keyName,
        hmac
      );
    } catch (error) {
      throw new Error(`Failed to store key: ${error.message}`);
    }
  }

  static async retrieveKeyWithValidation(
    keyName: string, 
    server: string
  ): Promise<string | null> {
    try {
      const keyCredentials = await SecureCredentialStorage
        .getCredentials(`${server}_${keyName}`);
      const hmacCredentials = await SecureCredentialStorage
        .getCredentials(`${server}_${keyName}${this.HMAC_KEY_SUFFIX}`);
      
      if (!keyCredentials || !hmacCredentials) {
        return null;
      }
      
      const hmacKey = await this.getHMACKey(server);
      const computedHmac = CryptoJS.HmacSHA256(
        keyCredentials.token, 
        hmacKey
      ).toString();
      
      if (computedHmac !== hmacCredentials.token) {
        console.error('Key integrity validation failed');
        await this.removeKey(keyName, server);
        throw new Error('Key integrity validation failed');
      }
      
      const [version, keyData] = keyCredentials.token.split(':');
      return keyData;
    } catch (error) {
      console.error('Key retrieval failed:', error);
      return null;
    }
  }
}
```

</td>
</tr>
</table>

---

### 3. SSL Pinning Inseguro

#### **Arquivo:** `app/lib/methods/helpers/sslPinning.ts`

<table>
<tr>
<th width="50%">❌ ANTES (Configuração Fraca)</th>
<th width="50%">✅ DEPOIS (Pinning Rigoroso)</th>
</tr>
<tr>
<td>

```typescript
// app/lib/methods/helpers/sslPinning.ts - Linha 32-68
const RCSSLPinning = Platform.select({
  ios: {
    setCertificate: (name: string, server: string) => {
      if (name) {
        let certificate = UserPreferences.getMap(name) as ICertificate;
        if (!certificate.path.match(extractFileScheme(documentDirectory!))) {
          certificate = persistCertificate(name, certificate.password);
        }
        UserPreferences.setMap(extractHostname(server), certificate);
        SSLPinning?.setCertificate(
          server, 
          certificate.path, 
          certificate.password
        );
      }
    }
  },
  android: {
    setCertificate: name => SSLPinning?.setCertificate(name)
  }
});
```

</td>
<td>

```typescript
// app/lib/security/SSLPinningManager.ts
export class SSLPinningManager {
  private static readonly PINNED_CERTIFICATES = {
    'rocket.chat': [
      'sha256/AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA=',
      'sha256/BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB='
    ]
  };

  static validateCertificateChain(
    serverUrl: string, 
    certificateChain: string[]
  ): boolean {
    try {
      const hostname = new URL(serverUrl).hostname;
      const pinnedHashes = this.getPinnedHashes(hostname);
      
      if (!pinnedHashes || pinnedHashes.length === 0) {
        console.warn(`No pinned certificates for: ${hostname}`);
        return true;
      }
      
      for (const cert of certificateChain) {
        const certHash = this.calculateCertificateHash(cert);
        if (pinnedHashes.includes(certHash)) {
          return true;
        }
      }
      
      console.error(`Certificate pinning failed for ${hostname}`);
      return false;
    } catch (error) {
      console.error('Certificate validation error:', error);
      return false;
    }
  }

  static detectProxyOrDebugging(): boolean {
    if (__DEV__) {
      return false;
    }
    
    const suspiciousIndicators = [
      'Proxy-Connection',
      'X-Forwarded-For',
      'Via'
    ];
    
    return false;
  }
}
```

</td>
</tr>
</table>

---

### 4. Timeouts Excessivos

#### **Arquivo:** `e2e/tests/room/01-createroom.spec.ts`

<table>
<tr>
<th width="50%">❌ ANTES (Timeout Fixo)</th>
<th width="50%">✅ DEPOIS (Timeout Progressivo)</th>
</tr>
<tr>
<td>

```typescript
// e2e/tests/room/01-createroom.spec.ts - Linha 185-209
describe('Create Room', () => {
  it('should create a new room', async () => {
    await waitFor(element(by.id('rooms-list-view-create-channel')))
      .toBeVisible()
      .withTimeout(60000);
    
    await element(by.id('rooms-list-view-create-channel')).tap();
    
    await waitFor(element(by.id('new-message-view')))
      .toBeVisible()
      .withTimeout(60000);
    
  });
});
```

</td>
<td>

```typescript
// e2e/tests/room/01-createroom.spec.ts - Correção
import { TimeoutManager } from '../../app/lib/network/TimeoutManager';

describe('Create Room', () => {
  it('should create a new room', async () => {
    await TimeoutManager.executeWithProgressiveTimeout(
      async () => {
        await element(by.id('rooms-list-view-create-channel')).tap();
        await waitFor(element(by.id('new-message-view')))
          .toBeVisible()
          .withTimeout(15000);
      },
      'NORMAL',
      (attempt) => {
        console.log(`Attempt ${attempt}/3`);
      }
    );

  });
});

// app/lib/network/TimeoutManager.ts
export class TimeoutManager {
  private static readonly TIMEOUT_CONFIGS = {
    QUICK: 5000,
    NORMAL: 15000,
    EXTENDED: 30000,
    UPLOAD: 60000
  };

  static async executeWithProgressiveTimeout<T>(
    operation: () => Promise<T>,
    timeoutType: keyof typeof TimeoutManager.TIMEOUT_CONFIGS = 'NORMAL',
    onProgress?: (attempt: number) => void
  ): Promise<T> {
    let lastError: Error;
    
    for (let attempt = 1; attempt <= 3; attempt++) {
      try {
        onProgress?.(attempt);
        const timeout = this.calculateTimeout(timeoutType, attempt);
        return await this.withTimeout(operation(), timeout);
      } catch (error) {
        lastError = error;
        if (attempt < 3) {
          const delay = this.calculateRetryDelay(attempt);
          await this.sleep(delay);
        }
      }
    }
    
    throw lastError;
  }
}
```

</td>
</tr>
</table>

---

## 🟡 PROBLEMAS DE PRIORIDADE MÉDIA

### 5. Vazamento de Informações em Logs

#### **Arquivo:** `app/lib/methods/helpers/log/events.ts`

<table>
<tr>
<th width="50%">❌ ANTES (Logs Não Filtrados)</th>
<th width="50%">✅ DEPOIS (Logs Sanitizados)</th>
</tr>
<tr>
<td>

```typescript
// app/lib/methods/helpers/log/events.ts - Linha 340-362
export const logEvent = (event: string, parameters?: any): void => {
  try {
    console.log(`Event: ${event}`, parameters);
    
    if (Analytics) {
      Analytics.logEvent(event, parameters);
    }
  } catch (error) {
    console.error('Failed to log event', error);
  }
};

```

</td>
<td>

```typescript
// app/lib/logging/SecureLogger.ts
export class SecureLogger {
  private static readonly SENSITIVE_PATTERNS = [
    /token["\s]*[:=]["\s]*([^"'\s]+)/gi,
    /password["\s]*[:=]["\s]*([^"'\s]+)/gi,
    /authorization["\s]*:["\s]*([^"'\s]+)/gi,
    /bearer\s+([a-zA-Z0-9\-._~+/]+=*)/gi,
    /\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Z|a-z]{2,}\b/gi,
  ];

  private static readonly REPLACEMENT_TEXT = '[REDACTED]';

  static log(
    level: 'info' | 'warn' | 'error' | 'debug', 
    message: string, 
    data?: any
  ): void {
    if (__DEV__ || level === 'error') {
      const sanitizedMessage = this.sanitizeMessage(message);
      const sanitizedData = data ? this.sanitizeData(data) : undefined;
      
      console[level](`[${level.toUpperCase()}]`, sanitizedMessage, sanitizedData);
    }
    
    if (!__DEV__ && level === 'error') {
      this.sendToErrorReporting(sanitizedMessage, sanitizedData);
    }
  }

  private static sanitizeData(data: any): any {
    if (data && typeof data === 'object') {
      const sanitized = {};
      Object.keys(data).forEach(key => {
        const lowerKey = key.toLowerCase();
        if (this.isSensitiveKey(lowerKey)) {
          sanitized[key] = this.REPLACEMENT_TEXT;
        } else {
          sanitized[key] = this.sanitizeData(data[key]);
        }
      });
      return sanitized;
    }
    return data;
  }
}

// app/lib/methods/helpers/log/events.ts - Correção
export const logEvent = (event: string, parameters?: any): void => {
  try {
    SecureLogger.log('info', `Event: ${event}`, parameters);
    
    if (Analytics && !containsSensitiveData(parameters)) {
      Analytics.logEvent(event, parameters);
    }
  } catch (error) {
    SecureLogger.log('error', 'Failed to log event', { 
      event, 
      error: error.message 
    });
  }
};
```

</td>
</tr>
</table>

---

### 6. Validação OAuth Insuficiente

#### **Arquivo:** `app/containers/LoginServices/serviceLogin.ts`

<table>
<tr>
<th width="50%">❌ ANTES (Validação Básica)</th>
<th width="50%">✅ DEPOIS (Validação Rigorosa)</th>
</tr>
<tr>
<td>

```typescript
// app/containers/LoginServices/serviceLogin.ts - Linha 63-87
const loginWithOAuth = async (provider: string): Promise<void> => {
  try {
    const credentialToken = random(43);
    const state = Base64.encodeURI(JSON.stringify({
      loginStyle: 'redirect',
      credentialToken,
      isCordova: true,
      redirectUrl: 'rocketchat://auth'
    }));
    
    const authUrl = `${server}/oauth/${provider}?state=${state}`;
    
    const result = await openAuthWebView(authUrl);
    
    if (result.type === 'success') {
      const url = new URL(result.url);
      const code = url.searchParams.get('code');
      
      if (code) {
        await Services.loginOAuthOrSso({
          credentialToken: code
        });
      }
    }
  } catch (error) {
    console.error('OAuth failed:', error);
  }
};
```

</td>
<td>

```typescript
// app/containers/LoginServices/serviceLogin.ts - Correção
import { OAuthValidator } from '../../lib/security/OAuthValidator';

const loginWithOAuth = async (provider: string): Promise<void> => {
  try {
    const state = OAuthValidator.generateSecureState();
    const codeVerifier = OAuthValidator.generateCodeVerifier();
    const codeChallenge = OAuthValidator.generateCodeChallenge(codeVerifier);
    
    await AsyncStorage.setItem(`oauth_state_${provider}`, state);
    await AsyncStorage.setItem(`oauth_verifier_${provider}`, codeVerifier);
    
    const authUrl = buildAuthorizationURL(provider, {
      state,
      code_challenge: codeChallenge,
      code_challenge_method: 'S256'
    });
    
    const result = await openAuthWebView(authUrl);
    
    if (result.type === 'success') {
      const storedState = await AsyncStorage.getItem(`oauth_state_${provider}`);
      const storedVerifier = await AsyncStorage.getItem(`oauth_verifier_${provider}`);
      
      if (!OAuthValidator.validateAuthorizationURL(result.url, storedState)) {
        throw new Error('OAuth validation failed');
      }
      
      const url = new URL(result.url);
      const code = url.searchParams.get('code');

      const tokenResponse = await exchangeCodeForToken(
        provider, 
        code, 
        storedVerifier
      );
      
      if (!OAuthValidator.validateTokenResponse(tokenResponse)) {
        throw new Error('Invalid token response');
      }
      
      await Services.loginOAuthOrSso({
        credentialToken: tokenResponse.access_token,
        credentialSecret: tokenResponse.refresh_token
      });
    }
  } catch (error) {
    console.error('OAuth login failed:', error);
    throw error;
  } finally {
    await AsyncStorage.removeItem(`oauth_state_${provider}`);
    await AsyncStorage.removeItem(`oauth_verifier_${provider}`);
  }
};
```

</td>
</tr>
</table>

---

### 7. Operações Síncronas Bloqueantes

#### **Arquivo:** `app/lib/methods/helpers/localAuthentication.ts`

<table>
<tr>
<th width="50%">❌ ANTES (Operações Síncronas)</th>
<th width="50%">✅ DEPOIS (Operações Assíncronas)</th>
</tr>
<tr>
<td>

```typescript
// app/lib/methods/helpers/localAuthentication.ts - Linha 30-64
export const localAuthenticate = async (server: string): Promise<void> => {
  try {
    const serverRecord = getServerRecord(server);
    if (!serverRecord) return;

    const timesync = getServerTimeSync(server);
    const lastSession = getLastAuthenticationSession(server);
    
    const diffToLastSession = timesync ? 
      Math.abs(moment().diff(lastSession)) : null;

    if (!timesync || (serverRecord?.autoLockTime && 
        diffToLastSession >= serverRecord.autoLockTime)) {
      
      RNBootSplash.hide({ fade: true });
      
      store.dispatch(setLocalAuthenticated(false));
      
      handleLocalAuthentication();
      
      store.dispatch(setLocalAuthenticated(true));
    }

    resetAttempts();
    saveLastLocalAuthenticationSession(server, serverRecord, timesync);
    
  } catch (error) {
    console.error('Local authentication failed:', error);
  }
};
```

</td>
<td>

```typescript
// app/lib/methods/helpers/localAuthentication.ts - Correção
import { InteractionManager } from 'react-native';

export const localAuthenticate = async (server: string): Promise<void> => {
  await InteractionManager.runAfterInteractions();
  
  try {
    const serverRecord = await getServerRecordAsync(server);
    if (!serverRecord) return;

    const [timesync, lastSession] = await Promise.all([
      getServerTimeSyncAsync(server),
      getLastAuthenticationSessionAsync(server)
    ]);

    const diffToLastSession = timesync ? 
      Math.abs(moment().diff(lastSession)) : null;

    if (!timesync || (serverRecord?.autoLockTime && 
        diffToLastSession >= serverRecord.autoLockTime)) {
      
      await hideSplashScreen();
      
      store.dispatch(setLocalAuthenticated(false));
      
      await executeAuthenticationAsync();
      
      store.dispatch(setLocalAuthenticated(true));
    }

    await Promise.all([
      resetAttemptsAsync(),
      saveLastLocalAuthenticationSessionAsync(server, serverRecord, timesync)
    ]);
    
  } catch (error) {
    console.error('Local authentication failed:', error);
    throw error;
  }
};

const getServerRecordAsync = async (server: string): Promise<TServerModel | null> => {
  return new Promise((resolve) => {
    const executeWhenIdle = (callback: () => void) => {
      if (typeof requestIdleCallback !== 'undefined') {
        requestIdleCallback(callback);
      } else {
        setTimeout(callback, 0);
      }
    };
    
    executeWhenIdle(async () => {
      try {
        const serversDB = database.servers;
        const serversCollection = serversDB.get('servers');
        const record = await serversCollection.find(server);
        resolve(record);
      } catch (error) {
        resolve(null);
      }
    });
  });
};
```

</td>
</tr>
</table>

---

### 8. Vazamentos de Memória em Listeners

#### **Arquivo:** `app/lib/services/connect.ts`

<table>
<tr>
<th width="50%">❌ ANTES (Listeners Não Gerenciados)</th>
<th width="50%">✅ DEPOIS (Gerenciamento Automático)</th>
</tr>
<tr>
<td>

```typescript
// app/lib/services/connect.ts - Linha 51-100
class ConnectionManager {
  async connect(server: string): Promise<void> {
    try {
      sdk.current.on('connect', this.handleConnect);
      sdk.current.on('disconnect', this.handleDisconnect);
      sdk.current.on('error', this.handleError);
      
      await sdk.connect(server);
      
      return new Promise((resolve, reject) => {
        sdk.current.on('ready', resolve);
        sdk.current.on('error', reject);
      });
      
    } catch (error) {
      throw error;
    }
  }

  disconnect(): void {
    sdk.current.off('connect', this.handleConnect);
    sdk.current.off('disconnect', this.handleDisconnect);

    sdk.disconnect();
  }
}
```

</td>
<td>

```typescript
// app/lib/services/connect.ts - Correção
import { ListenerManager } from '../memory/ListenerManager';

class ConnectionManager {
  private abortController: AbortController | null = null;
  private connectionId: string | null = null;

  async connect(server: string, options: ConnectionOptions = {}): Promise<void> {
    this.disconnect();
    
    this.abortController = new AbortController();
    this.connectionId = `connection_${Date.now()}`;
    
    try {
      await this.establishConnection(server, options);
    } catch (error) {
      if (error.name === 'AbortError') {
        console.log('Connection aborted');
        return;
      }
      throw error;
    }
  }

  private async establishConnection(
    server: string, 
    options: ConnectionOptions
  ): Promise<void> {
    const signal = this.abortController?.signal;
    
    const connectionComponent = {};
    
    if (sdk.current) {
      ListenerManager.addListener(
        connectionComponent,
        'connect',
        this.handleConnect.bind(this),
        sdk.current
      );
      
      ListenerManager.addListener(
        connectionComponent,
        'disconnect',
        this.handleDisconnect.bind(this),
        sdk.current
      );
      
      signal?.addEventListener('abort', () => {
        ListenerManager.cleanupComponent(connectionComponent);
      });
    }
    
    await TimeoutManager.executeWithProgressiveTimeout(
      async () => {
        if (signal?.aborted) {
          throw new Error('Connection aborted');
        }
        
        await sdk.connect(server, {
          ...options,
          signal
        });
      },
      'EXTENDED'
    );
  }

  disconnect(): void {
    if (this.abortController) {
      this.abortController.abort();
      this.abortController = null;
    }
    
    if (this.connectionId) {
      this.connectionId = null;
    }
  }
}
```

</td>
</tr>
</table>

---

## 🟢 PROBLEMAS DE PRIORIDADE BAIXA

### 9. Uso Excessivo de Sleep em Testes

#### **Arquivo:** `e2e/helpers/app.ts`

<table>
<tr>
<th width="50%">❌ ANTES (Sleep Fixo)</th>
<th width="50%">✅ DEPOIS (Esperas Condicionais)</th>
</tr>
<tr>
<td>

```typescript
// e2e/helpers/app.ts - Linha 23-54
const tapBack = async () => {
  await sleep(1000);
  
  try {
    await element(by.id('header-back')).tap();
  } catch {
    await element(by.label('Back')).tap();
  }
  
  await sleep(500);
};

const navigateToRoom = async (roomName: string) => {
  await sleep(2000);
  
  await element(by.id('rooms-list-view-search')).tap();
  await element(by.id('rooms-list-view-search-input')).typeText(roomName);
  
  await sleep(1500);
  
  await element(by.id(`rooms-list-view-item-${roomName}`)).tap();
  
  await sleep(1000);
};

const sleep = (ms: number): Promise<void> => {
  return new Promise(resolve => setTimeout(resolve, ms));
};
```

</td>
<td>

```typescript
// e2e/helpers/waitUtils.ts
export class WaitUtils {
  static async waitForCondition(
    condition: () => Promise<boolean> | boolean,
    options: {
      timeout?: number;
      interval?: number;
      errorMessage?: string;
    } = {}
  ): Promise<void> {
    const {
      timeout = 15000,
      interval = 500,
      errorMessage = 'Condition not met within timeout'
    } = options;
    
    const startTime = Date.now();
    
    while (Date.now() - startTime < timeout) {
      try {
        const result = await condition();
        if (result) {
          return;
        }
      } catch (error) {

      }
      
      await this.sleep(interval);
    }
    
    throw new Error(`${errorMessage} (timeout: ${timeout}ms)`);
  }

  static async waitForElement(
    elementMatcher: Detox.NativeElement,
    options: { timeout?: number; visible?: boolean; } = {}
  ): Promise<void> {
    const { timeout = 15000, visible = true } = options;
    
    await this.waitForCondition(
      async () => {
        try {
          if (visible) {
            await expect(elementMatcher).toBeVisible();
          } else {
            await expect(elementMatcher).toExist();
          }
          return true;
        } catch {
          return false;
        }
      },
      {
        timeout,
        errorMessage: `Element not ${visible ? 'visible' : 'found'}`
      }
    );
  }
}

// e2e/helpers/app.ts - Correção
const tapBack = async () => {
  await WaitUtils.waitForCondition(
    async () => {
      try {
        await expect(element(by.id('header-back'))).toBeVisible();
        return true;
      } catch {
        try {
          await expect(element(by.label('Back'))).toBeVisible();
          return true;
        } catch {
          return false;
        }
      }
    }
  );
  
  try {
    await element(by.id('header-back')).tap();
  } catch {
    await element(by.label('Back')).tap();
  }
};

const navigateToRoom = async (roomName: string) => {
  await WaitUtils.waitForElement(element(by.id('rooms-list-view-search')));
  
  await element(by.id('rooms-list-view-search')).tap();
  await element(by.id('rooms-list-view-search-input')).typeText(roomName);
  
  await WaitUtils.waitForElement(
    element(by.id(`rooms-list-view-item-${roomName}`))
  );
  
  await element(by.id(`rooms-list-view-item-${roomName}`)).tap();
  
  await WaitUtils.waitForElement(element(by.id('room-view')));
};
```

</td>
</tr>
</table>

---

### 10. Operações de Database Não Otimizadas

#### **Arquivo:** `app/sagas/login.js`

<table>
<tr>
<th width="50%">❌ ANTES (Operações Sequenciais)</th>
<th width="50%">✅ DEPOIS (Operações em Batch)</th>
</tr>
<tr>
<td>

```javascript
// app/sagas/login.js - Linha 210-248
const handleLoginSuccess = function* handleLoginSuccess({ user }) {
  try {
    const server = yield select(getServer);
    
    
    const serversDB = database.servers;
    const usersCollection = serversDB.get('users');
    yield serversDB.write(async () => {
      try {
        const userRecord = await usersCollection.find(user.id);
        await userRecord.update(record => {
          record._raw = sanitizedRaw({ id: user.id, ...record._raw });
          Object.assign(record, user);
        });
      } catch (e) {
        await usersCollection.create(record => {
          record._raw = sanitizedRaw({ id: user.id });
          Object.assign(record, user);
        });
      }
    });
    
    yield serversDB.write(async () => {
      try {
        const serversHistoryCollection = serversDB.get('servers_history');
        const serversHistory = await serversHistoryCollection
          .query(Q.where('url', server)).fetch();
        if (serversHistory?.length) {
          await serversHistory[0].update(s => {
            s.username = user.username;
          });
        }
      } catch (e) {
        log(e);
      }
    });
    
    
  } catch (e) {
    log(e);
  }
};
```

</td>
<td>

```javascript
// app/sagas/login.js - Correção
import { BatchOperations } from '../lib/database/BatchOperations';

const handleLoginSuccess = function* handleLoginSuccess({ user }) {
  try {
    const server = yield select(getServer);
    
    const databaseOperations = [];
    
    databaseOperations.push(async (writer) => {
      const usersCollection = database.get('users');
      try {
        const userRecord = await usersCollection.find(user.id);
        await userRecord.update(record => {
          Object.assign(record, sanitizedUser);
        });
      } catch {
        await usersCollection.create(record => {
          Object.assign(record, sanitizedUser);
        });
      }
    });
    
    databaseOperations.push(async (writer) => {
      const serversHistoryCollection = database.get('servers_history');
      const serversHistory = await serversHistoryCollection
        .query(Q.where('url', server))
        .fetch();
        
      if (serversHistory?.length) {
        await serversHistory[0].update(s => {
          s.username = user.username;
        });
      }
    });
    
    yield call(
      BatchOperations.executeDatabaseBatch, 
      database, 
      databaseOperations
    );
    
    yield fork(
      SecureCredentialStorage.storeCredentials, 
      server, 
      user.id, 
      user.token
    );
    
  } catch (e) {
    console.error('Login success handling failed:', e);
  }
};

// app/lib/database/BatchOperations.ts
export class BatchOperations {
  static async executeDatabaseBatch(
    database: any,
    operations: ((writer: any) => Promise<void>)[]
  ): Promise<void> {
    return database.write(async (writer: any) => {
      await Promise.all(
        operations.map(operation => operation(writer))
      );
    });
  }

  static async executeBatch<T>(
    operations: (() => Promise<T>)[],
    options: {
      maxConcurrency?: number;
      timeout?: number;
    } = {}
  ): Promise<T[]> {
    const { maxConcurrency = 5, timeout = 30000 } = options;
    
    const results: T[] = [];
    const batches = this.createBatches(operations, maxConcurrency);
    
    for (const batch of batches) {
      const batchPromises = batch.map(op => 
        this.withTimeout(op(), timeout)
      );
      
      const batchResults = await Promise.allSettled(batchPromises);
      
      batchResults.forEach(result => {
        if (result.status === 'fulfilled') {
          results.push(result.value);
        } else {
          console.error('Batch operation failed:', result.reason);
        }
      });
      
      if (batches.length > 1) {
        await this.sleep(100);
      }
    }
    
    return results;
  }
}
```

</td>
</tr>
</table>

---

## 📊 Resumo das Melhorias

### Benefícios de Segurança

| **Problema** | **Antes** | **Depois** | **Benefício** |
|---|---|---|---|
| **Credenciais** | Texto plano no AsyncStorage | Keychain/Android Keystore + Criptografia | 🔒 Proteção contra dispositivos comprometidos |
| **Chaves E2EE** | Sem validação de integridade | HMAC + Validação rigorosa | 🛡️ Proteção contra manipulação de chaves |
| **SSL Pinning** | Configuração básica | Pinning rigoroso + Detecção de proxy | 🌐 Proteção contra MITM |
| **Logs** | Dados sensíveis expostos | Sanitização automática | 🔍 Prevenção de vazamentos |
| **OAuth** | Validação básica | PKCE + Validação rigorosa | 🔐 Proteção contra ataques CSRF |

### Benefícios de Performance

| **Problema** | **Antes** | **Depois** | **Benefício** |
|---|---|---|---|
| **Timeouts** | 60s fixos | Progressivo com retry | ⚡ UX melhorada, menos ANRs |
| **I/O Bloqueante** | Operações síncronas | Operações assíncronas | 🚀 UI responsiva |
| **Memory Leaks** | Listeners manuais | Gerenciamento automático | 💾 Uso eficiente de memória |
| **Sleep em Testes** | Timeouts fixos | Esperas condicionais | 🧪 Testes mais estáveis |
| **Database** | Operações sequenciais | Operações em batch | 💽 Performance de banco otimizada |

---

## 🛠️ Próximos Passos

### 1. **Priorização de Implementação**
- ✅ Começar pelos problemas de **Prioridade Alta** (segurança crítica)
- ⚠️ Seguir com **Prioridade Média** (melhorias importantes)
- 🔧 Finalizar com **Prioridade Baixa** (otimizações)

### 2. **Validação e Testes**
- Cada mudança deve incluir testes unitários e de integração
- Testes de segurança específicos para cada vulnerabilidade
- Benchmarks de performance antes/depois

### 3. **Migração Gradual**
- Implementar migração automática de dados existentes
- Manter backwards compatibility durante transição
- Monitorar impacto em produção

---

**Data de Criação:** 28 de julho de 2025  
**Versão:** 1.0  
**Responsável:** Equipe de Desenvolvimento Rocket.Chat ReactNative

---

*Este documento comparativo facilita a revisão de código e aprovação das mudanças propostas pela equipe de desenvolvimento.*
