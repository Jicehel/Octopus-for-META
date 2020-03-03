#ifndef RUNSTATE
#define RUNSTATE

#include "BarcaSprite.h"
#include "OctopusSprite.h"
#include "SubSprite.h"

// -------------------------------------------------------------------------
// Run state
// -------------------------------------------------------------------------

class RunState {

  public:

    void update() {
      if (life == 0) {
        gameState = GameState::gameOver;
        if (score > highScore) {
          highScore = score;
        }
      }
      else {
        // -------------------------------------------------------------------------
        // User interaction
        // -------------------------------------------------------------------------

        moveTick++;

        if ((gb.buttons.released(BUTTON_A)) || (gb.buttons.released(BUTTON_B))) {
          gameState = GameState::pauseScreen;
          gb.sound.play("sons\Pause.wav");
        }

        // -------------------------------------------------------------------------
        // Anim objects
        // -------------------------------------------------------------------------
        if ((gb.buttons.pressed(BUTTON_LEFT)) && (catchGold == false) && (moveSubButton == true) && (hited==false)) {
          if (subMove >= 2) {
            // if sub don't have gold
            --subMove;
            gb.sound.playTick();
          } else if (subGold == true && subMove >= 1) {
            //if sub have gold
            --subMove;
            gb.sound.playTick();
          }
          
        }
      }

      if ((gb.buttons.pressed(BUTTON_RIGHT)) && (catchGold == false) && (moveSubButton == true) && (hited==false)) {
        if (subMove < 5 && subGold == false) {
          ++subMove;
          gb.sound.playTick();
        } else if (subMove == 5 && subGold == false) {
          catchGold = true;
          subMove = 6;
          gb.sound.playTick();
        } else if (subMove == 5 && subGold == true) {
          catchGold = true;
          subMove = 6;
          gb.sound.playTick();
        } else if (subMove < 5 && subGold == true) {
          ++subMove;
          gb.sound.playTick();
        }
      }


      if (subMove == 0 && subGold == true && inGold == false) {
        inGold = true;
      } else {
        inGold = false;
      }

      if (subMove > 0 && life == 3 && subGold == false) {
        barcaState = 1;
      } else if (subMove == 0 && life == 3 && subGold == true) {
        barcaState = 2;
      } else if (subMove > 0 && life == 2 && subGold == false) {
        barcaState = 4;
      } else if (subMove == 0 && life == 2 && subGold == true) {
        barcaState = 5;
      } else if (subMove > 0 && life == 1 && subGold == false) {
        barcaState = 7;
      } else if (subMove == 0 && life == 1 && subGold == true) {
        barcaState = 8;
      }

      if ((debug == true) && (subMove > 0) && (oldVal != subMove)) {
        SerialUSB.print("11111 - Avant --");
        SerialUSB.print("subMove = ");
        SerialUSB.println(subMove);
        SerialUSB.print("diverToShow = ");
        SerialUSB.println(diverToShow);
        oldVal = subMove;
      }
      Octopus();
      Barca();
      if (subMove < 6) {
        Sub();
      } else {
        animSubGold();
      }
      OctopusCollision();
      RunState::draw();

      if (moveTick > maxTick) moveTick = 0;
    }


    void draw() {
      // Declares a pointer that will alternate between the two memory buffers
      uint16_t* buffer;
      // Declares the top border of current slice
      uint8_t sliceY;
      // Go through each slice one by one
      for (uint8_t sliceIndex = 0; sliceIndex < slices; sliceIndex++) {
        // Buffers are switched according to the parity of sliceIndex
        buffer = sliceIndex % 2 == 0 ? buffer1 : buffer2;
        // Top border of the current slice is calculated
        sliceY = sliceIndex * sliceHeight;

        // Starts by drawing the background
        memcpy(buffer, background + sliceY * screenWidth, 2 * screenWidth * sliceHeight);


        // draw divers on barca
        if (show_SpriteDiver1)drawSprite(spriteDiver1, sliceY, buffer);
        if (show_SpriteDiver1_arm1)drawSprite(spriteDiver1_arm1, sliceY, buffer);
        if (show_SpriteDiver1_arm2)drawSprite(spriteDiver1_arm2, sliceY, buffer);
        if (show_SpriteDiver2)drawSprite(spriteDiver2, sliceY, buffer);
        if (show_SpriteDiver3)drawSprite(spriteDiver3, sliceY, buffer);

        // draw octopus leg 1
        if (show_octopus_leg1_1)drawSprite(spriteOctopus_leg1_1, sliceY, buffer);
        if (show_octopus_leg1_2)drawSprite(spriteOctopus_leg1_2, sliceY, buffer);
        if (show_octopus_leg1_3)drawSprite(spriteOctopus_leg1_3, sliceY, buffer);
        if (show_octopus_leg1_4)drawSprite(spriteOctopus_leg1_4, sliceY, buffer);

        // draw octopus leg 2
        if (show_octopus_leg2_1)drawSprite(spriteOctopus_leg2_1, sliceY, buffer);
        if (show_octopus_leg2_2)drawSprite(spriteOctopus_leg2_2, sliceY, buffer);
        if (show_octopus_leg2_3)drawSprite(spriteOctopus_leg2_3, sliceY, buffer);
        if (show_octopus_leg2_4)drawSprite(spriteOctopus_leg2_4, sliceY, buffer);
        if (show_octopus_leg2_5)drawSprite(spriteOctopus_leg2_5, sliceY, buffer);

        // draw octopus leg 3
        if (show_octopus_leg3_1)drawSprite(spriteOctopus_leg3_1, sliceY, buffer);
        if (show_octopus_leg3_2)drawSprite(spriteOctopus_leg3_2, sliceY, buffer);
        if (show_octopus_leg3_3)drawSprite(spriteOctopus_leg3_3, sliceY, buffer);
        if (show_octopus_leg3_4)drawSprite(spriteOctopus_leg3_4, sliceY, buffer);

        // draw octopus leg 4
        if (show_octopus_leg4_1)drawSprite(spriteOctopus_leg4_1, sliceY, buffer);
        if (show_octopus_leg4_2)drawSprite(spriteOctopus_leg4_2, sliceY, buffer);
        if (show_octopus_leg4_3)drawSprite(spriteOctopus_leg4_3, sliceY, buffer);

        // draw the 3 parts of the diver if needed
        if (diverToShow > 0) drawSprite(diver[diverToShow - 1], sliceY, buffer);
        if (diverArmToShow > 0) drawSprite(diverArm[diverArmToShow - 1], sliceY, buffer);
        if (diverBagToShow > 0) drawSprite(diverBag[diverBagToShow - 1], sliceY, buffer);

        // Draw score
        drawScore(score, sliceY, buffer);

        // Verify that previous buffer has been sent to the DMA controller
        if (sliceIndex != 0) waitForPreviousDraw();
        // Then send the current buffer
        customDrawBuffer(0, sliceY, buffer, screenWidth, sliceHeight);
      }

      // Wait until the DMA transfer is completed before entering the next cycle
      waitForPreviousDraw();
    }
};

RunState runState;

#endif
