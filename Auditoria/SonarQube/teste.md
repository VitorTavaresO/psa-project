```python
<!-- ANTES - Vulnerabilidade (possibilidade DDos) -->
export const getMessageUrlRegex = () => {
	return /https?:\/\/(www\.)?[-a-zA-Z0-9@:%._\+~#=]{1,256}\.[a-zA-Z0-9()]{1,6}\b([-a-zA-Z0-9()@:%_\+.~#?&//=]*)/g;
};

<!-- DEPOIS -->

```
