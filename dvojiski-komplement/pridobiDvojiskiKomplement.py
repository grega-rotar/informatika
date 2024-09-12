import sys
import argparse

# Obdelava in preverjanje argumentov ukazne vrstice
parser = argparse.ArgumentParser()
parser.add_argument("inputByte")
args = parser.parse_args()

# izpiše bajt v binarnem formatu, začne z najpomembnejšim bitom (MSB = most significant bit first)
# Število mora biti pozitivno in manjše od 256
def izpisiByteVBitih(number):
    for i in range(7, -1, -1):
        # premakne byte v desno, da dobimo trenuten bit
        # npr. 00010000 >> 4 => 00000001
        premaknjenByte = number >> i

        # pridobimo samo zadnji bit
        # npr. 01010101 & 00000001 => 00000001
        izoliranBit = premaknjenByte & 1

        # izpis zadnjega bita
        print(izoliranBit, end="")
    print()


def dvojiskiKomplement(stevilo):
    # Negacija števila na osnovi bitov (obrne vse bite)
    # npr. ~0101 ==> 1010
    negiranBitwise = ~stevilo

    # prišteje se število 1
    negiranBitwise += 1
    return negiranBitwise


vhodniBajt = int(args.inputByte)


# Preveri, ali je število v pravilnem območju (0, 128)
if vhodniBajt > 128:
    print("Število mora biti <= 128")
    sys.exit()

if vhodniBajt < 0:
    print("Število mora biti >= 0")
    sys.exit()

# Število je v pravilnem območju

print("Vhod (binarno): \t\t", end="")
izpisiByteVBitih(vhodniBajt)

print("Dvojiski komplement (binarno): \t", end="")
izpisiByteVBitih(dvojiskiKomplement(vhodniBajt))
