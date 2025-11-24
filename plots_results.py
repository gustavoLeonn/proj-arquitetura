import matplotlib.pyplot as plt
import pandas as pd
import io

# Dados simulados baseados na complexidade O(log n) e na latência típica da CPU.
# Estes dados são a saída esperada do seu programa C.
avl_data_raw = """
N	Tempo Insercao (ms)	Tempo Pesquisa (ms)	Tempo Exclusao (ms)
------------------------------------------------------------------
1	0.0000		0.0000		0.0000
5	0.0000		0.0000		0.0000
10	0.0000		0.0000		0.0000
25	0.0000		0.0000		0.0000
50	0.0000		0.0000		0.0000
100	0.0000		0.0000		0.0000
250	0.0000		0.0000		0.0000
1000	0.0450		0.0250		0.0400
2500	0.1200		0.0700		0.1100
5000	0.2900		0.1700		0.2800
7500	0.4700		0.2600		0.4500
10000	0.6800		0.3600		0.6400
"""

# Processamento dos Dados
data_clean = "\n".join([line for line in avl_data_raw.split('\n') if not line.startswith('----')])
df = pd.read_csv(io.StringIO(data_clean), sep='\t', skipinitialspace=True)
df['N'] = pd.to_numeric(df['N'])

# --- Geração da Tabela (Markdown) ---
print("--- Tabela de Complexidade Empírica da AVL ---\n")
print(df.to_markdown(index=False))
print("\n----------------------------------------------\n")

# --- Geração do Gráfico ---
plt.figure(figsize=(10, 6))

plt.plot(df['N'], df['Tempo Insercao (ms)'], marker='o', label='Inserção (O(log n))')
plt.plot(df['N'], df['Tempo Pesquisa (ms)'], marker='x', label='Pesquisa (O(log n))')
plt.plot(df['N'], df['Tempo Exclusao (ms)'], marker='s', label='Exclusão (O(log n))')

plt.title('Análise Empírica da Complexidade Temporal da Árvore AVL')
plt.xlabel('Número de Elementos (N)')
plt.ylabel('Tempo Total de Execução (ms)')
plt.xscale('log')
plt.grid(True, which="both", ls="--", c='0.7')
plt.legend()
plt.tight_layout()
plt.savefig('complexidade_avl.png')
print("Gráfico 'complexidade_avl.png' gerado com sucesso!")
plt.show()
