/** big thanks to...
 * the c standerd libary
 * example images created in PixiEditor
 * or/then converted using https://cloudconvert.com/
 * big help from https://www-user.tu-chemnitz.de/~heha/hs/chm/petzold.chm/petzoldi/ch15b.htm
 * bitmap documentation https://en.wikipedia.org/wiki/BMP_file_format#File_structure
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ASCII_CHAR_TACK 45 //magic val for the ascii char "-"
#define ASCII_CHAR_TAB  9  //magic val for the ascii char "\t", "  ", or "tab"
#define ASCII_CHAR_STAR 42 //magic val for the ascii char "*"

typedef long long int int64;
typedef unsigned char BYTE;
typedef short int WORD;
typedef long int int32;

enum flags{ //enum for ascii values
  a = 97, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z,
  zero = 48, one, two, three, four, five, six, seven, eight, nine,
};

typedef struct {
  int32 w; //width
  int32 h; //hight
  int32 c; //compression method
  int32 offset; //offset into pixel array
  int32 fSize; //file size
  int32 iSize; //image size: might not use
  int32 pSize; //bytes per pixal, doubles as formate of pixals
  int32 pixMetx; //stores the pixels per meter when printing
  int32 pixMety; //stores the pixels per meter when printing
  int32* key; //a key to decrypt any secret message in the file
  BYTE* pixals; //reference to a pixal array
} bitmap;

char* testpath[] = { 
  "C:\\coding projects\\SDL\\steghid\\image\\examples\\aidesktopimg.bmp",
  "C:\\coding projects\\SDL\\steghid\\image\\examples\\aiimg1.bmp",
  "C:\\coding projects\\SDL\\steghid\\image\\examples\\Bmp_format2.bmp",
  "C:\\coding projects\\SDL\\steghid\\image\\examples\\brainmatter.bmp",
  "C:\\coding projects\\SDL\\steghid\\image\\examples\\test image.bmp",
  "C:\\coding projects\\SDL\\steghid\\image\\examples\\test image#2.bmp",
  "C:\\coding projects\\SDL\\steghid\\image\\examples\\uw_a04_1.bmp",
  "C:\\coding projects\\SDL\\steghid\\image\\examples\\uw_b01_2.bmp",
};
char testData[] = "this is a secert message";
int noise = 0;

void printPixalArr(bitmap* b, int p/*bool*/){ //debuging tool
  printf(
    "hight: %ld, Width: %ld, compression: %ld, fileSize: %lX, imageSize: %ld, size per pixal: %ld.\n\n", 
    b->h, b->w, b->c, b->fSize, b->iSize, b->pSize);
  if(p == 1){
    for (int i = 0; i < b->iSize; i++){
      for (int ii = 0; ii < (b->pSize/8); ii++){
        printf("%02X:", b->pixals[i+ii]);
      }
      printf("\n");
    }
  } else if (p == 2){
    FILE* print = fopen64("print.txt", "w");
    fprintf(print,
      "hight: %ld, Width: %ld, compression: %ld, fileSize: %lX, imageSize: %ld, size per pixal: %ld.\n\n", 
      b->h, b->w, b->c, b->fSize, b->iSize, b->pSize);
    for (int i = 0; i < b->iSize; i++){
      for (int ii = 0; ii < (b->pSize/8); ii++){
        fprintf(print, "%02X:", b->pixals[i+ii]);
      }
      fprintf(print, "\n");
  }
    fclose(print);
  }
}
int32 readIntoBuffer(FILE* file, int byteOffset){ //reads a byte offset into a buffer than flips the byte order from little indian to big.
  BYTE buf[byteOffset];
  for (int i = 0; i < byteOffset; i++){
    buf[i] = fgetc(file);
  }
  int32 t = 0;
  for (int i = 0; i < byteOffset; i++){
    t = t | (buf[i] << (8u * i));
  }
  return t;
}
bitmap readbmp(char* path){
  FILE* image = fopen64(path, "rb");
  bitmap bmp;
  bmp.pixals = NULL;
  bmp.key = NULL;
  if(fgetc(image) != 0x42 || fgetc(image) != 0x4D){
    fprintf(stderr, "file %s wasn't reconized as a bitmap file\n", path);
    exit(1);
  }
  bmp.fSize = readIntoBuffer(image, 4);
  if (readIntoBuffer(image, 2) != 0x00 || readIntoBuffer(image, 2) != 0x00){
    fprintf(stderr, "An error in the bitmap formating was found in file %s.\n", path);
    exit(1);
  }
  bmp.offset = readIntoBuffer(image, 4); 
  if (bmp.offset-14 != readIntoBuffer(image, 4)){} //leave here
  bmp.w = readIntoBuffer(image, 4);
  bmp.h = readIntoBuffer(image, 4);
  if (readIntoBuffer(image, 2) != 0x01){ 
    fprintf(stderr, "An error in the bitmap formating was found in file %s.\n", path);
    exit(1);
  }
  bmp.pSize = readIntoBuffer(image, 2);
  bmp.c = readIntoBuffer(image, 4);
  if(bmp.c != 0x00){ //checks for the type of conpression used
    switch (bmp.c){
      case 3:
        int huffman = 1;
          break;
      case 4:
        printf("JPEG type compression\n");
          break;
      case 5:
        printf("PNG type compression\n");
          break;
      case 11:
        printf("warning:\n\tunable to access windows specified compression algorithm\n");
          break;
      case 1:
      case 2:
      case 6:
      case 12:
      case 13:
        printf("unsuported compression algorithm.\ncase %ld\n", bmp.c);
        exit(0);
        break;
      default:
        printf("unrecognized compression algorithm.");
        exit(-1);
        break;
    }
  }
  bmp.iSize = readIntoBuffer(image, 4);
  fseek(image, 16, SEEK_CUR);
  bmp.iSize = bmp.h * bmp.w;
  bmp.pixals = (BYTE*)calloc(bmp.iSize*bmp.pSize, sizeof(BYTE));
  for (int32 i = 0; i < bmp.iSize*(bmp.pSize/8); i++){
    bmp.pixals[i] = (BYTE)fgetc(image);
  }
  int32 sig;
  sig = readIntoBuffer(image, 4); // null buffure
  sig = readIntoBuffer(image, 4);
  if(sig == 0x464F5552){
    int32 len = readIntoBuffer(image, 4);
    int32 bytePer = readIntoBuffer(image, 2);
    bmp.key = (int32*)malloc(sizeof(int32) * len);
    if(bmp.key == NULL){
      perror("something went wrong");
      exit(1);
    }
    for (int32 i = 0; i < len; i++){
      bmp.key[i] = readIntoBuffer(image, bytePer);
    }
  }
  fclose(image);
  return bmp;
}
void pushIntoBuffure(FILE* file, int32 buf, int numbyte){ //flips the byte order in a buffer from little indian to big then writes the buffer into a file.
  for(int i = 0; i < numbyte*8; i+=8){
    BYTE t = (buf >> i) & 0xff;
    fputc(t, file);
  }
}
void createBmp(char* name, bitmap* bmp){
  FILE* out = fopen64(name, "wb");
  if (out == NULL) {
    perror("failed to create and output file");
    exit(1);
  }

  fputs("BM", out);
  pushIntoBuffure(out, bmp->fSize, 4);
  for (int i = 0; i < 4; i++){
    fputc((char)0x00, out); 
  } 
  pushIntoBuffure(out, bmp->offset, 4); 
  pushIntoBuffure(out, bmp->offset-14, 4);
  pushIntoBuffure(out, bmp->w, 4);
  pushIntoBuffure(out, bmp->h, 4);
  fputc((char)0x01, out), fputc((char)0x00, out);
  pushIntoBuffure(out, bmp->pSize, 2);
  pushIntoBuffure(out, bmp->c, 4);
  pushIntoBuffure(out, (bmp->iSize*(bmp->pSize/8)), 4);
  for (int i = 0; i < 16; i++){
    fputc((char)0x00, out); 
  }
  for (int32 i = 0; i < bmp->iSize*(bmp->pSize/8); i++){ 
    fputc(bmp->pixals[i], out);
  }
  if (bmp->key){
    size_t len = 0;
    while(bmp->key[len]) len++;
    for (int i = 0; i < 4; i++) fputc(0x0, out);
    int bytePerKey = 2;
    for (size_t i = 0; i < len; i++){
      if (bmp->key[i] >= 0x10000){
        bytePerKey = 4;
        break;
      }
    }
    fputs("RUOF", out); 
    pushIntoBuffure(out, len, 4);
    pushIntoBuffure(out, bytePerKey, 2);
    for (size_t i = 0; i < len; i++){
      pushIntoBuffure(out, bmp->key[i], bytePerKey);
    }
  }
  fclose(out); 
}
void manipulateBitmapImage(bitmap* bmp, char* data){
  srand(time(NULL));
  const size_t limit = 1000;
  size_t iii = 0, i = 0, len = 0;
  while(data[len])len++;
  int32 loc[len];
  int32 imageSize = bmp->iSize;
  int32 buf[limit];
  for (i = 0; i < len; i++) loc[i] = 0;
  for (i = 0; i < len; i++){
    for (int32 ii = 0; ii < imageSize; ii++){
      if(iii >= limit) break;
      if(data[i] == bmp->pixals[ii]){
        buf[iii++] = ii;
      }
    }
    if(iii != 0){
      loc[i] = buf[rand()%iii];
      iii = 0;
    } else {
      loc[i] = -1;
    }
  } 
  for (i = 0; i < len; i++){
    if(loc[i] == -1){
      for (int32 ii = 0; ii < imageSize; ii++){
        if(iii >= limit) break;
        int eval = data[i]-bmp->pixals[ii];
        if(abs(eval) <= 10) {
          bmp->pixals[ii] += eval;
          buf[iii++] = ii;
        }
      }
      if(iii != 0){
        loc[i] = buf[rand()%iii];
        iii = 0;
      }
    }
    if(loc[i] == -1) {
      for (int32 ii = 0; ii < imageSize; ii++){
        if(iii >= limit) break;
        int eval = data[i]-bmp->pixals[ii];
        if(abs(eval) <= 20) {
          bmp->pixals[ii] += eval;
          buf[iii++] = ii;
        }
      }
      if(iii != 0){
        loc[i] = buf[rand()%iii];
        iii = 0;
      }
    }
  }
  bmp->key = (int32*)calloc(len+1, sizeof(int32));
  for (size_t i = 0; i < len; i++){
    if(loc[i] == -1) {
      fprintf(stderr, "Image is too uniform. Please select a different image.\n");
      printf("%d\n", i);
      exit(0);
    }
    bmp->key[i] = loc[i];
  }
  printf("\n");
}
void displayHelpInfo(){
  FILE* info = fopen("C:\\coding projects\\SDL\\steghid\\image\\info.txt", "r");
  if(info == NULL){    
    perror("failed to open the help page\n");
    exit(1);
  }
  char ch;
  do {
    ch = fgetc(info);
    if(ch == ASCII_CHAR_STAR) ch = ASCII_CHAR_TAB;
    printf("%c", ch);
  } while (ch != EOF);
  fclose(info);
  exit(0);
}
int main(int argc, char const *argv[]){
  printf("\n");
  if(argc == 1){
    printf("An argument to this program is expected.\ntype in '-h' or '-help' for additional infomation.\n\n");
    return 0;
  }
  
  bitmap image;
  char* data = NULL;
  char imagePath[_MAX_PATH];
  char outName[_MAX_PATH];
  
  for (int i = 1; i < argc; i++){
    if(argv[i][0] == ASCII_CHAR_TACK){
      switch ((int)argv[i][1]){
        case h: //help
          displayHelpInfo();
              break;
        case c: //convert
          printf("coming soon...TM");
            break;
        case f: //file
            int len = 0;
            while(argv[i+1][len]) len++;
            data = (char*)malloc(sizeof(char) * _MAX_PATH);
            
              break;
        case e: //encrypted/decrypted
          if (argv[i+1][0] == one){
            if(imagePath[0] != 0){
              image = readbmp(imagePath);  
              if(image.key == NULL){
                printf("there was no data encrypted in this file");
                return 0;
              }
              len = 0;
              while (image.key[len]) len++;
              for (int ii = 0; ii < len; ii++){
                printf("%c", image.pixals[image.key[ii]]);
                // printf("%d", image.key[ii]); 
              }
                printf("\n"); 
            } else {
              printf("no image to analyze");
            }
          } else {

          }
              break;  
        case d: //message/data
          len = 0;
          while(argv[i+1][len]) len++;
          data = (char*)malloc(sizeof(char) * len);
          for (int ii = 0; ii < len; ii++){
            data[i] = argv[i+1][ii];
          }
          i++;
              break;
        case t: //testmode
            int tpath;
            switch (argv[i+1][0]){
            case zero: tpath = 0;
                  break;
            case one: tpath = 1;
                  break;
            case two: tpath = 2;
                  break;
            case three: tpath = 3;
                  break;
            case four: tpath = 4;
                  break;
            case five: tpath = 5;
                  break;
            case six: tpath = 6;
                  break;
            case seven: tpath = 7;
                  break;
            default: printf("not an valiable test path");
                  break;
            }
            image = readbmp(testpath[tpath]);
            manipulateBitmapImage(&image, testData);
            createBmp("test.bmp", &image);
            //todo: demo
            return 0;
        case p: //altas
            printf("todo: demo.");
              break;
        case n: //noise
          noise += ((int)argv[i+1][0])-48;
          i++;
            break;
        case a: //altas
          len = 0;
          while(argv[i][len]) len++;
          if(len > _MAX_PATH) ;
              break;
        default:
          break;
      }
    } else {
      int len = 0;
      while(argv[i][len]) len++;
      if(len > _MAX_PATH){
        fprintf(stderr, "Path: %s is too big.\n\n Nice try ;)\n", argv[i]);
        return 0;
      }
      for (int ii = 0; ii < len; ii++){
        imagePath[ii] = argv[i][ii];
      }
    }
  }

  if(data == NULL) {
    printf("no data was provied.\n");
    return 0;
  }
  if(imagePath[0]){
    printf("no image path was provided.\n");
    printf("%s\n", imagePath);
    return 0;
  }
  if(outName[0]){
    char buf[22];
    sprintf(buf, "out#%lld.bmp", time(NULL));
    for (int i = 0; i < sizeof(buf)/sizeof(buf[0]); i++){
      outName[i] = buf[i];
    }
  }

  manipulateBitmapImage(&image, data);
  createBmp(outName, &image);

  if(data != NULL) free(data); 
  printf("\n");
  return 0;
}

