#include <M5Unified.h>

M5Canvas canvas(&M5.Lcd);

int screenW, screenH;
const int groundHeight = 16;

float birdY, birdVelocity;
const float gravity = 0.5;
const float jumpStrength = -4.5;
int birdX;
const int birdRadius = 8;
int wingFrame = 0;
unsigned long lastWingTime = 0;

struct Pipe { float x; int gapY; bool scored; };
const int pipeCount = 3;
Pipe pipes[pipeCount];
const int pipeWidth = 26;
const int pipeCapHeight = 8;
const int pipeCapExtra = 4;
const int gapHeight = 50;
const float pipeSpeed = 2.2;

int score = 0, bestScore = 0;
float groundOffset = 0;

enum GameState { WAITING, PLAYING, GAMEOVER };
GameState state = WAITING;

void resetGame() {
  birdY = screenH / 2;
  birdVelocity = 0;
  score = 0;
  for (int i = 0; i < pipeCount; i++) {
    pipes[i].x = screenW + i * (screenW / pipeCount + 60);
    pipes[i].gapY = random(20, screenH - groundHeight - 20 - gapHeight);
    pipes[i].scored = false;
  }
}

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  M5.Lcd.setRotation(1);
  screenW = M5.Lcd.width();
  screenH = M5.Lcd.height();
  birdX = screenW / 4;
  canvas.createSprite(screenW, screenH);
  randomSeed(analogRead(0));
  resetGame();
}

void drawBackground() {
  canvas.fillScreen(0x5DA0EC);
  canvas.fillEllipse(30, 25, 12, 6, TFT_WHITE);
  canvas.fillEllipse(42, 22, 10, 5, TFT_WHITE);
  canvas.fillEllipse(150, 35, 14, 7, TFT_WHITE);
  canvas.fillEllipse(165, 32, 10, 5, TFT_WHITE);
}

void drawGround() {
  int gy = screenH - groundHeight;
  canvas.fillRect(0, gy, screenW, groundHeight, 0xDDBB55);
  canvas.fillRect(0, gy, screenW, 4, 0x4CAF50);
  for (int x = -((int)groundOffset % 12); x < screenW; x += 12) {
    canvas.drawFastVLine(x, gy + 5, groundHeight - 5, 0xB89B4A);
  }
}

void drawPipe(int px, int gapY) {
  int gy = screenH - groundHeight;
  canvas.fillRect(px, 0, pipeWidth, gapY - pipeCapHeight, 0x2ECC71);
  canvas.fillRect(px - pipeCapExtra, gapY - pipeCapHeight, pipeWidth + pipeCapExtra * 2, pipeCapHeight, 0x27AE60);
  int bottomY = gapY + gapHeight;
  canvas.fillRect(px, bottomY + pipeCapHeight, pipeWidth, gy - (bottomY + pipeCapHeight), 0x2ECC71);
  canvas.fillRect(px - pipeCapExtra, bottomY, pipeWidth + pipeCapExtra * 2, pipeCapHeight, 0x27AE60);
}

void drawBird(int x, int y) {
  bool wingUp = (wingFrame % 2 == 0);
  canvas.fillEllipse(x, y, birdRadius, birdRadius - 2, TFT_YELLOW);
  if (wingUp) {
    canvas.fillEllipse(x - 2, y - 2, 6, 3, 0xFFA500);
  } else {
    canvas.fillEllipse(x - 2, y + 3, 6, 3, 0xFFA500);
  }
  canvas.fillCircle(x + 4, y - 3, 2, TFT_WHITE);
  canvas.fillCircle(x + 5, y - 3, 1, TFT_BLACK);
  canvas.fillTriangle(x + birdRadius - 2, y, x + birdRadius + 6, y - 2, x + birdRadius + 6, y + 3, 0xFF6600);
}

void drawHUD() {
  canvas.setTextColor(TFT_WHITE, TFT_BLACK);
  canvas.setTextSize(2);
  canvas.setCursor(5, 5);
  canvas.print(score);
}

void drawWaiting() {
  drawBackground();
  drawGround();
  drawBird(birdX, (int)birdY);
  canvas.setTextColor(TFT_WHITE);
  canvas.setTextSize(2);
  canvas.setCursor(screenW/2 - 42, screenH/2 - 40);
  canvas.print("TAP A");
  canvas.setTextSize(1);
  canvas.setCursor(screenW/2 - 40, screenH/2 - 20);
  canvas.print("to start flying");
  canvas.pushSprite(0, 0);
}

void drawGame() {
  drawBackground();
  for (int i = 0; i < pipeCount; i++) {
    drawPipe((int)pipes[i].x, pipes[i].gapY);
  }
  drawGround();
  drawBird(birdX, (int)birdY);
  drawHUD();
  canvas.pushSprite(0, 0);
}

void drawGameOver() {
  drawBackground();
  drawGround();
  canvas.setTextColor(TFT_RED);
  canvas.setTextSize(2);
  canvas.setCursor(screenW/2 - 58, screenH/2 - 35);
  canvas.print("GAME OVER");
  canvas.setTextColor(TFT_WHITE);
  canvas.setTextSize(1);
  canvas.setCursor(screenW/2 - 40, screenH/2 - 5);
  canvas.print("Score: "); canvas.print(score);
  canvas.setCursor(screenW/2 - 40, screenH/2 + 10);
  canvas.print("Best: "); canvas.print(bestScore);
  canvas.setCursor(screenW/2 - 45, screenH/2 + 30);
  canvas.print("Tap A to retry");
  canvas.pushSprite(0, 0);
}

void loop() {
  M5.update();
  groundOffset += pipeSpeed;

  if (millis() - lastWingTime > 150) {
    wingFrame++;
    lastWingTime = millis();
  }

  if (state == WAITING) {
    drawWaiting();
    if (M5.BtnA.wasPressed()) state = PLAYING;
    delay(30);
    return;
  }

  if (state == GAMEOVER) {
    drawGameOver();
    if (M5.BtnA.wasPressed()) { resetGame(); state = WAITING; }
    delay(30);
    return;
  }

  if (M5.BtnA.wasPressed()) {
    birdVelocity = jumpStrength;
    M5.Speaker.tone(800, 40);
  }

  birdVelocity += gravity;
  birdY += birdVelocity;

  int groundY = screenH - groundHeight;
  bool collided = (birdY - birdRadius < 0) || (birdY + birdRadius > groundY);

  for (int i = 0; i < pipeCount; i++) {
    pipes[i].x -= pipeSpeed;
    if (pipes[i].x + pipeWidth < 0) {
      pipes[i].x = screenW;
      pipes[i].gapY = random(20, groundY - 20 - gapHeight);
      pipes[i].scored = false;
    }
    if (!pipes[i].scored && pipes[i].x + pipeWidth < birdX) {
      pipes[i].scored = true;
      score++;
      M5.Speaker.tone(1200, 30);
    }
    if (birdX + birdRadius > pipes[i].x - pipeCapExtra && birdX - birdRadius < pipes[i].x + pipeWidth + pipeCapExtra) {
      if (birdY - birdRadius < pipes[i].gapY || birdY + birdRadius > pipes[i].gapY + gapHeight)
        collided = true;
    }
  }

  if (collided) {
    M5.Speaker.tone(200, 300);
    if (score > bestScore) bestScore = score;
    state = GAMEOVER;
  }

  drawGame();
  delay(30);
}
