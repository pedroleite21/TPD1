struct conta {
    int id;
    float saldo;
};

struct transacao {
    int id;
    float saldo;
    int erro;
};

struct token {
    int conta_id;
    int token;
};

program BANCO_PROG 
{
    version BANCO_VERS {
        int abreConta(int) = 1;
        int fechaConta (int) = 2;
        int authConta(int) = 3;
        int deposito(transacao) = 4;
        int saque(transacao) = 5;
        float retornaSaldo(int) = 6;
        int checkSenha(token) = 7;
        int geraSenha (token) = 8;
    } = 1;
} = 0x12345000;

