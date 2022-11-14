

#include <stdbool.h>
#include <stdlib.h>

// Varje 4 bitar är positionen av ett block. De två första ger x-koordinat, de två sista ger y-koordinat.
// De allra två första bitarna är x-koordinaten av den första blocket (dens y-koordinaten är såklart högst upp på spelplanet).
// Rotationerna är också angivna i 4 bitar och anger hur varje blocks x- och y-koordinat ska roteras
//Här är bitrepresentationerna för de olika koordinaterna: 00 = -1, 01 = 0, 10 = 1. För form 3, and rotation 3 och 4: x: 00=-1 01=0 10=1 11=2, y: 00=-2 01=-1 10=0 11=1

                                                        //block1 block0   block2   block3   rot1   rot2   rot3   rot4
const int TForm0 = 0b010100011000101010001000010110; //00 0100 0110 0010 1010 0010 0001 0110
const int TForm1 = 0b011001000101011000001000010110; //01 1001 0001 0101 1000 0010 0001 0110
const int TForm2 = 0b010110010010100000100010110110; //01 0110 0100 1010 0000 1000 1011 0110
const int TForm3 = 0b010001100101110010100010110110; //01 0001 1001 0111 0010 1000 1011 0110

const int IForm0 = 0b010110010001000010100011011000; //00 0110 0100 0100 0010 1000 1101 1000
const int IForm1 = 0b010001100111100010100011011000; //10 0001 1001 1110 0010 1000 1101 1000

const int OForm = 0b010110001000100101010101100110; //00 0110 0010 0010 0101 0101 0110 0110
const int JForm0 = 0b010100011000110001010101100110; //01 0100 0110 0011 0001 0101 0110 0110
const int JForm1 = 0b011001000100011000010101100110; //01 1001 0001 0001 1000 0101 0110 0110
const int JForm2 = 0b010110010010011010010101100110; //01 0110 0100 1001 1010 0101 0110 0110
const int JForm3 = 0b010001100110110010010101100110; //01 0001 1001 1011 0010 0101 0110 0110

const int LForm0 = 0b010110010000011010010101100110; //01 0110 0100 0001 1010 0101 0110 0110
const int LForm1 = 0b010001100110010010010101100110; //01 0001 1001 1001 0010 0101 0110 0110
const int LForm2 = 0b010100011010110000010101100110; //01 0100 0110 1011 0000 0101 0110 0110
const int LForm3 = 0b011001000100111000010101100110; //01 1001 0001 0011 1000 0101 0110 0110

const int SForm0 = 0b010110000100011010010101100110; //01 0110 0001 0001 1010 0101 0110 0110
const int SForm1 = 0b011001011000111010010101100110; //01 1001 0110 0011 1010 0101 0110 0110

const int ZForm0 = 0b010100000100110000010101100110; //01 0100 0001 0011 0000 0101 0110 0110
const int ZForm1 = 0b010001011010110000010101100110; //01 0001 0110 1011 0000 0101 0110 0110

const int middleformx = 20;
const int middleformy = 5;

//klass för Block
typedef struct {
    int x;
    int y;
}Block;

//klass för Form
typedef struct {
    Block block[4];
    int state[4];
    int currentState; //rotation
}Form;

//booelsk array för fallna block
bool fallenblocks[20][10];

//instansierar object form av klass Form
static Form form;

int nextform;

//extern för att kunna använda score och speed från andra filer
extern int score;
extern int speed;



void createForm(int random) {
    //skapar nästa form som ett random nummer, och tar in ett random nummer och bestämmer vilken form den ska vara utifrån cases.
    nextform = rand()&7;
    switch(random) {
    case 0:
    form.state[0] = JForm0;
    form.state[1] = JForm1;
    form.state[2] = JForm2;
    form.state[3] = JForm3;
    break;
    
    case 1:
    form.state[0] = LForm0;
    form.state[1] = LForm1;
    form.state[2] = LForm2;
    form.state[3] = LForm3;
    break;
    
    case 2:
    form.state[0] = SForm0;
    form.state[1] = SForm1;
    form.state[2] = SForm0;
    form.state[3] = SForm1;
    break;
    
    case 3:
    form.state[0] = ZForm0;
    form.state[1] = ZForm1;
    form.state[2] = ZForm0;
    form.state[3] = ZForm1;
    break;
    
    case 4:
    form.state[0] = TForm0;
    form.state[1] = TForm1;
    form.state[2] = TForm2;
    form.state[3] = TForm3;
    break;
    
    case 5:
    form.state[0] = IForm0;
    form.state[1] = IForm1;
    form.state[2] = IForm0;
    form.state[3] = IForm1;
    break;
    
    case 6:
    form.state[0] = OForm;
    form.state[1] = OForm;
    form.state[2] = OForm;
    form.state[3] = OForm;
    break;
    
    default:
    form.state[0] = IForm0;
    form.state[1] = IForm1;
    form.state[2] = IForm0;
    form.state[3] = IForm1;
    
    }
    
    //definierar vad block ska innehålla.
    form.block[1].x = middleformx-(((form.state[0])>>28)&0b0011);
    form.block[1].y = middleformy;
    form.block[0].x = (form.block[1].x)+(((form.state[0])>>26)&0b0011) - 1;
    form.block[0].y = (form.block[1].y)+(((form.state[0])>>24)&0b0011) - 1;
    form.block[2].x = (form.block[1].x)+(((form.state[0])>>22)&0b0011) - 1;
    form.block[2].y = (form.block[1].y)+(((form.state[0])>>20)&0b0011) - 1;
    form.block[3].x = (form.block[1].x)+(((form.state[0])>>18)&0b0011) - 1;
    form.block[3].y = (form.block[1].y)+(((form.state[0])>>16)&0b0011) - 2;
    
    
    form.currentState = 0;
    
        
}

//ritar formerna
void drawForm(void) {
    int i;
    int j;
    for (i=0;i<4;i++) {
        drawBlock(form.block[i].x, form.block[i].y);
    }
}

//ritar nästa form
void drawNextForm(void) {
    switch(nextform) {
        
        //J
        case 0:
        drawBlock(23,6);
        drawBlock(23,5);
        drawBlock(23,7);
        drawBlock(22,7);
        break;
        
        //L
        case 1:
        drawBlock(23,6);
        drawBlock(23,5);
        drawBlock(23,7);
        drawBlock(22,5);
        break;
        
        //S
        case 2:
        drawBlock(22,5);
        drawBlock(22,6);
        drawBlock(23,6);
        drawBlock(23,7);
        break;
        
        //Z
        case 3:
        drawBlock(23,5);
        drawBlock(23,4);
        drawBlock(22,5);
        drawBlock(22,6);
        break;
        
        //T
        case 4:
        drawBlock(23,7);
        drawBlock(22,6);
        drawBlock(23,6);
        drawBlock(23,5);
        break;
        
        //I
        case 5:
        drawBlock(23,5);
        drawBlock(23,4);
        drawBlock(23,6);
        drawBlock(23,7);
        break;
        
        //O
        case 6:
        drawBlock(22,6);
        drawBlock(23,6);
        drawBlock(22,5);
        drawBlock(23,5);
        break;
        
        default:
        drawBlock(23,5);
        drawBlock(23,4);
        drawBlock(23,6);
        drawBlock(23,7);
        
    }
}

//ritar fallna block om det finns ett block på det indexet.
void drawFallenblocks(void) {
    int x;
    int y;
    for(x = 0; x<20; x++){
        for(y = 0; y<10; y++){
            if(fallenblocks[x][y]){
                drawBlock(x+1, y+1);
            }
        }
    }
}

// när en rad fyllts med block tar vi bort alla block från den raden i fallenblocks
void breakLine(int line){
    int i;
    for(i = 0; i<10; i++){ 
        fallenblocks[line][i] = 0;
    }
    line++;
    for(line; line<20; line++){
        for(i = 0; i<10; i++){
            //if exists
            if(fallenblocks[line][i]){
                //move block downwards.
                fallenblocks[line][i]=0;
                fallenblocks[line-1][i]=1;
            }
        }
    }
    
    score += 10*(speed+1); //increment score
}

//resettta fallenblocks
void clearBlocks(void) {
    int i;
    int j;
    for(i=0;i<20;i++) {
        for (j=0;j<10;j++) {
            if (fallenblocks[i][j]) {
                fallenblocks[i][j] = 0;
            }
        }
    }
}

// kollar om någon rad är fylld
void checkLines(void){
    int line;
    int count;
    int y;
    for(line = 0; line<20; line++){
        count = 0;
        for(y = 0; y<10; y++){
            if(!(fallenblocks[line][y])){ 
                count++;
            }
        }
        if(count==0){ 
            breakLine(line);
            line--;
            }
    }
    
}

//kollar om en flyttning är tilåten, dvs ingenting i vägen
bool collisionCheck(void){
    int x;
    int y;
    int i;
    for(i = 0; i<4; i++){
        x = form.block[i].x;
        y = form.block[i].y;
        if(fallenblocks[x-1][y-1]){
        return 1;
        }
    }
    if (form.block[0].x == 0 || form.block[1].x == 0 || form.block[2].x == 0 || form.block[3].x == 0) {
    return 1;
    }
    
    else{
    return 0;
    }
    
}

//lägger till formen till fallenblocks om det har blivit kollision och skapar en ny form. Om en kollision sker direkt efter denna skapats så är spelet över.
void impact(void) {
    
    int i=0;
    int j=0;
    int xval;
    int yval;
    int q;
    
    for(q = 0; q<4; q++){
        xval = form.block[q].x;
        yval = form.block[q].y;
        fallenblocks[xval-1][yval-1]=1;
    }
    
    createForm(nextform);
    checkLines();
    
    if (collisionCheck()) {
        clearBlocks();
        gameover();
    } else {
        drawForm();
    }

}

// ser till så att förflyttning inte går mot vägg
bool wallCheck(void){
    //vänster vägg
    if (form.block[0].y <= 0 || form.block[1].y <= 0 || form.block[2].y <= 0 || form.block[3].y <= 0){
    return 1;
    }
    //höger vägg
    if (form.block[0].y >= 11 || form.block[1].y >= 11 || form.block[2].y >= 11 || form.block[3].y >= 11){
    return 1;
    }
    return 0;
}

//kollar om rotation är tillåten
bool rotationCheck(void){
    
    int pointX;
    int pointY;
    int i;
    
    for(i = 0; i<4; i++){
        int yOffset = 1;
        if(i>2){
            yOffset = 2;
        }
        pointX = (form.block[1].x)+(((form.state[form.currentState])>>(14-(i*4)))&0b0011) - 1;
        pointY = (form.block[1].y)+(((form.state[form.currentState])>>(12-(i*4)))&0b0011) - yOffset;
        
        if(fallenblocks[pointX-1][pointY-1] | pointX <= 0 | pointY <= 0 | pointY >= 11){
            return 1;
        }
    }
    return 0;
}

// förflyttar formen ett steg neråt
void fall(void) {
    int i;
    for (i=0;i<4;i++) {
        form.block[i].x -= 1;
    }
    
    if(collisionCheck()){
        // gör inte draget
        for (i=0;i<4;i++) {
            form.block[i].x += 1;    
        }
        impact();    
    }
    
}

// Flytta vänster
void moveLeft(void) {
    int i;
    for (i=0;i<4;i++) {
        form.block[i].y -= 1;    
    }    
    
    if(wallCheck()){
            
        //gör inte draget
        for (i=0;i<4;i++) {
            form.block[i].y += 1;
        }
            

    }else if(collisionCheck()){
        
        //gör inte draget
        for (i=0;i<4;i++) {
            form.block[i].y += 1;
        }
            
    }
        
}

//Flytta höger
void moveRight(void) {
    int i;
        for (i=0;i<4;i++) {
            form.block[i].y += 1;
        }
        
     
        if(wallCheck()){
            
            //gör inte draget.
            for (i=0;i<4;i++) {
                form.block[i].y -= 1;
            }
      
        }else if(collisionCheck()){
             //gör inte draget.
            for (i=0;i<4;i++) {
                form.block[i].y -= 1;
            }
        }
}
//Roterar formen och byter form motsvarande till rotationändringen
void rotate(void) {
    int i;
    
    if(form.currentState==3){
        form.currentState=0;
    }else{
        (form.currentState)++;
    }
    
    form.block[0].x = (form.block[1].x)+(((form.state[form.currentState])>>26)&0b0011) - 1;
    form.block[0].y = (form.block[1].y)+(((form.state[form.currentState])>>24)&0b0011) - 1;
    
    form.block[2].x = (form.block[1].x)+(((form.state[form.currentState])>>22)&0b0011) - 1;
    form.block[2].y = (form.block[1].y)+(((form.state[form.currentState])>>20)&0b0011) - 1;
    
    form.block[3].x = (form.block[1].x)+(((form.state[form.currentState])>>18)&0b0011) - 1;
    form.block[3].y = (form.block[1].y)+(((form.state[form.currentState])>>16)&0b0011) - 2;
    
    
    if(wallCheck() | collisionCheck() | rotationCheck()){
            
         //gör inte draget.
        if(form.currentState==0){
            form.currentState=3;
        }else{
            (form.currentState)--;
        }
            
        form.block[0].x = (form.block[1].x)+(((form.state[form.currentState])>>26)&0b0011) - 1;
        form.block[0].y = (form.block[1].y)+(((form.state[form.currentState])>>24)&0b0011) - 1;
    
        form.block[2].x = (form.block[1].x)+(((form.state[form.currentState])>>22)&0b0011) - 1;
        form.block[2].y = (form.block[1].y)+(((form.state[form.currentState])>>20)&0b0011) - 1;
    
        form.block[3].x = (form.block[1].x)+(((form.state[form.currentState])>>18)&0b0011) - 1;
        form.block[3].y = (form.block[1].y)+(((form.state[form.currentState])>>16)&0b0011) - 2;    
        
    }
        
        
        
}


