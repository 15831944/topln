/*   (C) Copyright 1998 SenseLock Technologies Inc. */

//////////////////////////////////////////////////////////
// D:\senseIII\vc\AutoC011.h : the file declare and state the key functions
//

#define	Read_Data	0	// It return the data you want to read
#define	Read_Addr	5	// The address of memory you want to read
#define	Write_Addr	0	// The address of memory you want to write
#define	Write_Data	1	// The data you want to write, it return the old data
#define	Encrypt_Highword_Data		1	// The high word of data you want to encrypt
#define	Encrypt_Lowword_Data		2	// The low word of data you want to encrypt
#define	Decrypt_Lowword_Data		0	// The low word of data you want to decrypt
#define	Decrypt_Highword_Data		2	// The high word of data you want to decrypt
#define	EncryptBak_Lowword_Data		3	// The low word of data you want to encrypt
#define	EncryptBak_Highword_Data		5	// The high word of data you want to encrypt
#define	DecryptBak_Highword_Data		0	// The high word of data you want to decrypt
#define	DecryptBak_Lowword_Data		1	// The low word of data you want to decrypt

typedef
	unsigned short Sense3AutoData[26];

#ifdef __cplusplus
extern "C" {
#endif

unsigned short OpenSense3(void);
void CloseSense3(void);

unsigned short ReadMemory0( // Read a word from memory in address
	Sense3AutoData p);

unsigned short ReadMemory1( // Read a word from memory in address
	Sense3AutoData p);

unsigned short ReadMemory2( // Read a word from memory in address
	Sense3AutoData p);

unsigned short Decrypt0( // Decrypt a encrypted word to its encrypting word
	Sense3AutoData p);

unsigned short Decrypt1( // Decrypt a encrypted word to its encrypting word
	Sense3AutoData p);

unsigned short Decrypt2( // Decrypt a encrypted word to its encrypting word
	Sense3AutoData p);

unsigned short Decrypt3( // Decrypt a encrypted word to its encrypting word
	Sense3AutoData p);

unsigned short Decrypt4( // Decrypt a encrypted word to its encrypting word
	Sense3AutoData p);

void DecryptBak( // Soft backup of decrypt function
	Sense3AutoData p);

unsigned short Encrypt0( // Encrypt a encrypting word to a encrypted word
	Sense3AutoData p);

unsigned short Encrypt1( // Encrypt a encrypting word to a encrypted word
	Sense3AutoData p);

unsigned short Encrypt2( // Encrypt a encrypting word to a encrypted word
	Sense3AutoData p);

void EncryptBak( // Soft backup of encrypt function
	Sense3AutoData p);

unsigned short WriteMemory0( // Write a word to memory in address
	Sense3AutoData p);

unsigned short WriteMemory1( // Write a word to memory in address
	Sense3AutoData p);

unsigned short WriteMemory2( // Write a word to memory in address
	Sense3AutoData p);

unsigned short WriteMemory3( // Write a word to memory in address
	Sense3AutoData p);

#ifdef __cplusplus
}
#endif
