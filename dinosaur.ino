#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define JUMPBUTTON_PIN 5
#define tickValue 200

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

int enemyHorPos = enemyHorStartPos;
int enemyVertPos = enemyVertStartPos;
int playerHorPos = playerHorStartPos;
int playerVertPos = playerVertStartPos;

const char enemyChar = 'Q';
const char playerChar = 'P';

bool enemyTouch = false;

void setup() {
  pinMode(JUMPBUTTON_PIN, INPUT_PULLUP);

  lcd.init();
  lcd.begin(16, 2);
  lcd.setBacklight(1);
  // startMenu();

  // Для отладки
  Serial.begin(9600);
}

void loop() {
  randomDistance = random(3, 7);
  randomDistance2 = random(1, 3);
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
  lcd.setCursor(0, 0);
  lcd.print("YOU DIE!");

  while(digitalRead(JUMPBUTTON_PIN) == HIGH) {
    Serial.println("Waiting for button signal");
  }

  enemyHorPos = enemyHorStartPos;
  playerVertPos = playerVertStartPos;
  loop();
  
}

void drawEnemy() {
  // Стирание предидущей позиции врага и отрисовка новой
  

  lcd.setCursor(enemyHorPos + 1, enemyVertPos);
  lcd.print(" ");
  lcd.setCursor(enemyHorPos + 1 + randomDistance, enemyVertPos - 1);
  lcd.print(" ");
  lcd.setCursor(enemyHorPos + 1 + randomDistance2, enemyVertPos);
  lcd.print(" ");

  // lcd.setCursor(0, 1);
  // lcd.print(" ");
  // lcd.setCursor(0, 0);
  // lcd.print(" ");

  lcd.setCursor(enemyHorPos, enemyVertPos);
  lcd.print(enemyChar);
  lcd.setCursor(enemyHorPos + randomDistance, enemyVertPos - 1);
  lcd.print(enemyChar);
  lcd.setCursor(enemyHorPos + randomDistance2, enemyVertPos);
  lcd.print(enemyChar);

  enemyHorPos--;

  if(enemyHorPos <= -9) {
    enemyHorPos = enemyHorStartPos;
    randomDistance = random(3, 7);
    randomDistance2 = random(7, 10);
  }
}

void drawPlayer() {
  lcd.setCursor(playerHorPos + 1, playerVertPos);
  lcd.print(playerChar);

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