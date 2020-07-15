#ifndef _ENCRYPTION_H
#define _ENCRYPTION_H



typedef struct
{
	unsigned long int erk[64];     /* encryption round keys */
	unsigned long int drk[64];     /* decryption round keys */
	int nr;             /* number of rounds */
}
context;

int  set_key( context *ctx, unsigned char *key, int nbits );
void encrypt( context *ctx, unsigned char input[16], unsigned char output[16] );
void decrypt( context *ctx, unsigned char input[16], unsigned char output[16] );

void encryptPW(unsigned char *input, unsigned int inputLen,unsigned char digest[16]);
#endif
