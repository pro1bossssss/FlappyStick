#include <M5Unified.h>

M5Canvas canvas(&M5.Lcd);

int screenW, screenH;

float birdY, birdVelocity;
const float gravity = 0.5;
const float jumpStrength = -4.5;
int birdX;
const int birdRadius = 6;

struct Pipe { float x; int gapY; bool scored; };
const int pipeCount = 3;
Pipe pipes[pipeCount];
const int pipeWidth = 20;
const int gapHeight = 45;
const float pipeSpeed = 2.0;

int score = 0, bestScore = 0;
enum GameState { WAITING, PLAYING, GAMEOVER };
GameState state = WAITING;

void resetGame() {
  birdY = screenH / 2;
  birdVelocity = 0;
  score = 0;
  for (int i = 0; i < pipeCount; i++) {
    pipes[i].x = screenW + i * (screenW / pipeCount + 40);
    pipes[i].gapY = random(20, screenH - 20 - gapHeight);
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

void drawWaiting() {
  canvas.fillScreen(TFT_BLACK);
  canvas.setTextColor(TFT_WHITE);
  canvas.setTextSize(2);
  canvas.setCursor(screenW/2 - 40, screenH/2 - 20);
  canvas.print("TAP A");
  canvas.setCursor(screenW/2 - 45, screenH/2 + 5);
  canvas.print("TO FLY");
  canvas.pushSprite(0, 0);
}

void drawGame() {
  canvas.fillScreen(TFT_CYAN);
  for (int i = 0; i < pipeCount; i++) {
    int px = (int)pipes[i].x;
    canvas.fillRect(px, 0, pipeWidth, pipes[i].gapY, TFT_GREEN);
    canvas.fillRect(px, pipes[i].gapY + gapHeight, pipeWidth, screenH - (pipes[i].gapY + gapHeight), TFT_GREEN);
  }
  canvas.fillCircle(birdX, (int)birdY, birdRadius, TFT_YELLOW);
  canvas.setTextColor(TFT_WHITE);
  canvas.setTextSize(2);
  canvas.setCursor(5, 5);
  canvas.print(score);
  canvas.pushSprite(0, 0);
}

void drawGameOver() {
  canvas.fillScreen(TFT_BLACK);
  canvas.setTextColor(TFT_RED);
  canvas.setTextSize(2);
  canvas.setCursor(screenW/2 - 55, screenH/2 - 25);
  canvas.print("GAME OVER");
  canvas.setTextColor(TFT_WHITE);
  canvas.setCursor(screenW/2 - 45, screenH/2);
  canvas.print("Score:"); canvas.print(score);
  canvas.setCursor(screenW/2 - 45, screenH/2 + 20);
  canvas.print("Best:"); canvas.print(bestScore);
  canvas.pushSprite(0, 0);
}

void loop() {
  M5.update();

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

  bool collided = (birdY - birdRadius < 0) || (birdY + birdRadius > screenH);

  for (int i = 0; i < pipeCount; i++) {
    pipes[i].x -= pipeSpeed;
    if (pipes[i].x + pipeWidth < 0) {
      pipes[i].x = screenW;
      pipes[i].gapY = random(20, screenH - 20 - gapHeight);
      pipes[i].scored = false;
    }
    if (!pipes[i].scored && pipes[i].x + pipeWidth < birdX) {
      pipes[i].scored = true;
      score++;
      M5.Speaker.tone(1200, 30);
    }
    if (birdX + birdRadius > pipes[i].x && birdX - birdRadius < pipes[i].x + pipeWidth) {
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
