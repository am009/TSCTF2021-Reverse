
extern uint8_t plain[];
extern uint8_t computed[16];

void aes_enc(uint8_t plain[]);
void aes_dec(uint8_t ciper[]);
void dec_init();

int lua_enc(lua_State* L);