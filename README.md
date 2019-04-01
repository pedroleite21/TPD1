# TPD1 Enunciado

Implementar um sistema distribuído baseado no modelo Cliente/Servidor na linguagem C utilizando RPC que simule a criação de contas de clientes e transações bancárias nestas contas. O sistema deve ter no mínimo 3 tipos de processos, administração, agência e caixa automático. Devem ser implementadas as seguintes funcionalidades:

* Processo Administração: realiza abertura e fechamento de contas (para agências), e também autentica que contas já existem (tanto para agências e caixas automáticos). A abertura deve ser uma operação garantidamente não-idempotente (semântica de execução exactely once);
* Processo Agência: Solicita abertura, autenticação e fechamento de contas e também pode solicitar depósito, retirada e consulta de saldo em conta existente. A abertura de conta, o depósito e a retirada devem ser operações garantidamente não-idempotentes (semântica de execução exactely once);
* Processo Caixa Automático: Solicita depósito, retirada e consulta de saldo em conta existente. As duas primeiras devem ser operações garantidamente não-idempotentes (semântica de execução exactely once) mesmo que ocorra algum erro na confirmação da operação (simular com injeção de falhas).

A avaliação do trabalho será feita com base no acompanhamento do desenvolvimento do trabalho em laboratório e no envio de um relatório técnico no moodle (uma página no formato .pdf, código em anexo sem limitação de tamanho).
