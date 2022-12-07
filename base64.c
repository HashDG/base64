#include <stdio.h>
#include <string.h>

char encoding_base64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
char decoding_base64[256];

void build_decoding_base64(void) {
	for (int i=0;i<64;i++) {
		decoding_base64[(unsigned char) encoding_base64[i]] = i;
	}
}

char * encode_b64(char * entree) {
	char resultat[strlen(entree)*4/3], * ptr = &entree[0], c;
	int tampon = 0, taille_tampon = 0, idx = 0;
	
	while(ptr[0] != '\0', c = ptr++[0]) {
		taille_tampon += 2;
		resultat[idx++] = encoding_base64[(c >> taille_tampon) | (tampon <<= 8 - taille_tampon) ];
		tampon = c&(1<<taille_tampon)-1;		
		if (taille_tampon == 6) {
			resultat[idx++] = encoding_base64[tampon];
			taille_tampon = tampon = 0;
		}
	}
	if (taille_tampon > 0) resultat[idx++] = encoding_base64[tampon <<= 6 - taille_tampon];
	while (idx%4 != 0) resultat[idx++] = '=';
	resultat[idx] = '\0';
	return strdup(resultat);
}

char * decode_b64(char * entree) {
	char resultat[strlen(entree)*3/4], * ptr = &entree[0], c;
	int tampon = 0, taille_tampon = 0, idx = 0;

	while(ptr[0] != '\0' && ptr[0] != '=', c = ptr++[0]) {
		if (taille_tampon == 0) {
			tampon = decoding_base64[c];
			taille_tampon = 6;
		} else {
			resultat[idx++] = (tampon << (8 - taille_tampon)) | (decoding_base64[c] >> (taille_tampon - 2) ), resultat[idx] = '\0';
			tampon = decoding_base64[c] & (1<<(taille_tampon-2))-1;
			taille_tampon -= 2;			
		}
	}
	
	return strdup(resultat);
}

int main(int argc, char * argv[]) {
	char * entree;
	if (argc > 1) {
		for (int i = (strstr(argv[1],"-d"))?2:1; i < argc; i++) {
			strcat(entree, argv[i]);
			strcat(entree, " ");
		}
		build_decoding_base64();
		entree[strlen(entree)-1]='\0';
		if (strstr(argv[1],"-d")) {
			printf("%s\n", decode_b64(entree));
		} else {
			printf("%s\n", encode_b64(entree));
		}
	} else {
		return 0;
	}
}
