#include <stdio.h>
#include <stdlib.h>

// izpiše bajt v binarnem formatu, začne z najpomembnejšim bitom (MSB = most significant bit first)
void izpisiByteVBitih(unsigned char byte) {
        int i;
        for (i = 7; i >= 0; i--) {
                // premakne byte v desno, da dobimo trenuten bit
                // npr. 00010000 >> 4 => 00000001
                char bit = (byte >> i);

                // pridobimo samo zadnji bit
                // npr. 01010101 & 00000001 => 00000001
                bit = bit & 1;

                // izpis zadnjega bita
                printf("%d", bit);
        }
        printf("\n");
}

unsigned char dvojiskiKomplement(unsigned char num) {
        // Negacija števila na osnovi bitov (obrne vse bite)
        // npr. ~0101 ==> 1010
        unsigned char negiranBitwise = ~num;

        // prišteje se število 1
        negiranBitwise += 1;
        return negiranBitwise;
}

int main(int argc, char *argv[]) {
        // Pretvori bajt (8-bitno celo število) v njegovo binarno predstavitev
        // v obliki dvojnega komplementa, obravnavajoč ga kot negativno število.
        if (argc < 2) {
                return 0;
        }

        char *pVhodnoStevilo = argv[1];
        int vhodnoStevilo = atoi(pVhodnoStevilo);

        if (vhodnoStevilo > 128) {
                printf("Value should be less or equal 128. Your value %d\n",
                       vhodnoStevilo);
                return 0;
        } else if (vhodnoStevilo < 0) {
                printf("Value should not be less or equal 0. Your value %d\n",
                       vhodnoStevilo);
                return 0;
        }

        unsigned char vhodniByte = (unsigned char)atoi(pVhodnoStevilo);

        printf("Vhod (binarno): ");
        izpisiByteVBitih(vhodniByte);
        printf("Dvojiski komplement (binarno): ");
        izpisiByteVBitih(dvojiskiKomplement(vhodniByte));
        return 0;
}