#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define JUMPBUTTON_PIN 5

#define enemyHorStartPos 15
#define enemyVertStartPos 1
#define playerHorStartPos 0
#define playerVertStartPos 1

LiquidCrystal_I2C lcd(0x27, 16, 2);

unsigned long timeStamp;
unsigned long jumpButtonClick;
unsigned long jumpButtonRelease;

int randomDistance = 5;
int randomDistance2 = 5;
int gameStartTimer = 3;

int tickValue = 200;

int enemyHorPos = enemyHorStartPos;
int enemyVertPos = enemyVertStartPos;
int playerHorPos = playerHorStartPos;
int playerVertPos = playerVertStartPos;

bool enemyTouch = false;

byte playerChar[] = {
  B11110,
  B01110,
  B11110,
  B01100,
  B11111,
  B01100,
  B11110,
  B01010
};

byte enemyChar[] = {
  B00100,
  B10100,
  B10101,
  B10101,
  B11111,
  B01110,
  B00110,
  B00110
};

byte birdChar[] = {
  B00010,
  B00110,
  B01100,
  B11110,
  B11110,
  B00000,
  B00000,
  B00000
};

byte hearthChar1[] = {
  B00000,
  B00110,
  B01111,
  B01110,
  B01111,
  B00110,
  B00011,
  B00001
};

byte hearthChar2[] = {
  B00000,
  B01100,
  B11110,
  B01110,
  B11110,
  B01100,
  B11000,
  B10000
};

byte hearthChar3[] = {
  B00000,
  B00000,
  B11011,
  B11111,
  B11111,
  B01110,
  B00100,
  B00000
};

void setup() {
  pinMode(JUMPBUTTON_PIN, INPUT_PULLUP);

  lcd.init();
  lcd.begin(16, 2);
  lcd.setBacklight(1);

  lcd.createChar(0, playerChar);
  lcd.createChar(1, enemyChar);
  lcd.createChar(2, birdChar);
  lcd.createChar(3, hearthChar1);
  lcd.createChar(4, hearthChar2);
  lcd.createChar(5, hearthChar3);


  Serial.begin(9600);
}

void loop() {
  randomDistance = random(3, 7);
  randomDistance2 = random(1, 3);

  lcd.setCursor(0, 0);
  lcd.print("\"Ardosaur\"");

  if(millis() - timeStamp > tickValue * 5) {
    timeStamp = millis();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Game start in...");

    while(gameStartTimer > -1) {
      if(millis() - timeStamp >= 1000) {
        lcd.setCursor(14,1);
        lcd.print(gameStartTimer);
      
        gameStartTimer--;
        timeStamp = millis();
      }
    }
    lcd.clear();
    mainGame();
  }
}

void mainGame() {
  // Цикл отрисовки персонажей и трека их движения
  while(checkDeath() == false) {
    if(millis() - timeStamp >= tickValue) {
      drawEnemy();
      timeStamp = millis();
    }

    if(digitalRead(JUMPBUTTON_PIN) == LOW) {
      //if(millis() - jumpButtonRelease > 500) {
        jumpButtonClick = millis();
        playerVertPos = 0;
      //}
    }

    if(digitalRead(JUMPBUTTON_PIN) == HIGH) {
      //if(millis() - jumpButtonClick > tickValue * 4) {
        jumpButtonRelease = millis();
        playerVertPos = 1;
      //}
    }
    
    drawPlayer();

    checkDeath();
  }

  lcd.clear();
  lcd.setCursor(8, 0);
  lcd.write(byte(5));
  delay(700);
  lcd.setBacklight(0);
  delay(300);
  lcd.clear();
  lcd.setBacklight(1);

  lcd.setCursor(7, 0);
  lcd.write(byte(3));
  lcd.setCursor(9, 0);
  lcd.write(byte(4));

  while(digitalRead(JUMPBUTTON_PIN) == HIGH) {
    Serial.println("Waiting for button signal");
  }

  enemyHorPos = enemyHorStartPos;
  playerVertPos = playerVertStartPos;
  loop();
  
}

void drawEnemy() {
  // Стирание предидущей позиции врага и отрисовка новой
  lcd.setCursor(enemyHorPos, enemyVertPos);
  lcd.write(byte(1));
  lcd.setCursor(enemyHorPos + randomDistance2, enemyVertPos);
  lcd.write(byte(1));
  lcd.setCursor(enemyHorPos + randomDistance, enemyVertPos - 1);
  lcd.write(byte(2));

  lcd.setCursor(enemyHorPos + 1, enemyVertPos);
  lcd.print(" ");
  lcd.setCursor(enemyHorPos + 1 + randomDistance2, enemyVertPos);
  lcd.print(" ");
  lcd.setCursor(enemyHorPos + 1 + randomDistance, enemyVertPos - 1);
  lcd.print(" ");

  enemyHorPos--;

  if(enemyHorPos <= -9) {
    enemyHorPos = enemyHorStartPos;
    randomDistance = random(3, 7);
    randomDistance2 = random(7, 10);
  }
}

void drawPlayer() {
  lcd.setCursor(playerHorPos + 1, playerVertPos);
  lcd.write(byte(0));

  // Стирание игрока по вертикали
  lcd.setCursor(playerHorPos + 1, playerVertPos - 1);
  lcd.print(" ");

  lcd.setCursor(playerHorPos + 1, playerVertPos + 1);
  lcd.print(" ");

  lcd.setCursor(0, 1);
  lcd.print(" ");
  lcd.setCursor(0, 0);
  lcd.print(" ");

}

bool checkDeath() {
  if(playerHorPos == enemyHorPos && playerVertPos == enemyVertPos || playerHorPos == enemyHorPos + randomDistance && playerVertPos == enemyVertPos - 1 || playerHorPos == enemyHorPos + randomDistance2 && playerVertPos == enemyVertPos) {
    enemyTouch = true;
    return true;
  }
  return false;
}
