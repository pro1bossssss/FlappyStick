#include <M5Unified.h>
#include "sprites.h"

M5Canvas canvas(&M5.Lcd);

int screenW, screenH;
const int groundHeight = 16;
const uint16_t TRANSPARENT_KEY = 0x0000;

float birdY, birdVelocity;
const float gravity = 0.5;
const float jumpStrength = -4.5;
int birdX;
const int birdRadius = 9;
int wingFrame = 0;
unsigned long lastWingTime = 0;

struct Pipe { float x; int gapY; bool scored; };
const int pipeCount = 3;
Pipe pipes[pipeCount];
const int pipeWidth = pipe_tile_w;
const int pipeCapHeight = 6;
const int pipeCapExtra = 4;
const int gapHeight = 52;
const float pipeSpeed = 2.2;

int score = 0, bestScore = 0;
float bgOffset = 0;
float groundOffset = 0;

enum GameState { WAITING, PLAYING, GAMEOVER };
GameState state = WAITING;

int safeMaxGapY(int groundY) {
  int minGapY = 14;
  int maxGapY = groundY - 14 - gapHeight;
  if (maxGapY < minGapY) maxGapY = minGapY + 1;
  return maxGapY;
}

void resetGame() {
  birdY = screenH / 2;
  birdVelocity = 0;
  score = 0;
  int groundY = screenH - groundHeight;
  for (int i = 0; i < pipeCount; i++) {
    pipes[i].x = screenW + i * (screenW / pipeCount + 70);
    pipes[i].gapY = random(14, safeMaxGapY(groundY));
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
  int step = bg_tile_w;
  int off = ((int)bgOffset) % step;
  for (int x = -off; x < screenW; x += step) {
    canvas.pushImage(x, 0, bg_tile_w, bg_tile_h, bg_tile);
  }
}

void drawGround() {
  int gy = screenH - groundHeight;
  canvas.fillRect(0, gy, screenW, groundHeight, canvas.color565(221,187,85));
  canvas.fillRect(0, gy, screenW, 4, canvas.color565(76,175,80));
  int off = ((int)groundOffset) % 12;
  for (int x = -off; x < screenW; x += 12) {
    canvas.drawFastVLine(x, gy + 5, groundHeight - 5, canvas.color565(184,155,74));
  }
}

void drawPipeColumn(int px, int topH, int bottomStartY, int bottomH) {
  uint16_t capColor = canvas.color565(70, 150, 60);

  canvas.setClipRect(px, 0, pipeWidth, topH);
  for (int y = topH - pipe_tile_h; y > -pipe_tile_h; y -= pipe_tile_h) {
    canvas.pushImage(px, y, pipe_tile_w, pipe_tile_h, pipe_tile);
  }
  canvas.clearClipRect();
  if (topH >= pipeCapHeight) {
    canvas.fillRect(px - pipeCapExtra, topH - pipeCapHeight, pipeWidth + pipeCapExtra*2, pipeCapHeight, capColor);
  }

  canvas.setClipRect(px, bottomStartY, pipeWidth, bottomH);
  for (int y = bottomStartY; y < bottomStartY + bottomH; y += pipe_tile_h) {
    canvas.pushImage(px, y, pipe_tile_w, pipe_tile_h, pipe_tile);
  }
  canvas.clearClipRect();
  if (bottomH >= pipeCapHeight) {
    canvas.fillRect(px - pipeCapExtra, bottomStartY, pipeWidth + pipeCapExtra*2, pipeCapHeight, capColor);
  }
}

void drawBird(int x, int y) {
  const uint16_t* frame; int fw, fh;
  int f = wingFrame % 3;
  if (f == 0) { frame = bird_f0; fw = bird_f0_w; fh = bird_f0_h; }
  else if (f == 1) { frame = bird_f1; fw = bird_f1_w; fh = bird_f1_h; }
  else { frame = bird_f2; fw = bird_f2_w; fh = bird_f2_h; }
  canvas.pushImage(x - fw/2, y - fh/2, fw, fh, frame, TRANSPARENT_KEY);
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
  int groundY = screenH - groundHeight;
  for (int i = 0; i < pipeCount; i++) {
    int px = (int)pipes[i].x;
    int topH = pipes[i].gapY;
    int bottomStartY = pipes[i].gapY + gapHeight;
    int bottomH = groundY - bottomStartY;
    if (bottomH < 0) bottomH = 0;
    drawPipeColumn(px, topH, bottomStartY, bottomH);
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
  bgOffset += pipeSpeed * 0.4;

  if (millis() - lastWingTime > 120) { wingFrame++; lastWingTime = millis(); }

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
      pipes[i].gapY = random(14, safeMaxGapY(groundY));
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
