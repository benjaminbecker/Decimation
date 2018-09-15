#include <Arduino.h>
#include "AudioStream.h"
#include "arm_math.h"


#define FILTER_TAP_NUM 20

int16_t filter_taps[FILTER_TAP_NUM] =
{
    0, 1, -6, -54, 18, 326,
    -14, -1178, -6, 5001, 8209, 5001,
    -6, -1178, -14, 326, 18, -54,
    -6, 1
};


int16_t input[AUDIO_BLOCK_SAMPLES] __attribute__ ( ( aligned ( 4 ) ) );
int16_t output[AUDIO_BLOCK_SAMPLES/4] __attribute__ ( ( aligned ( 4 ) ) );

void setup() {
  // init success variable to false
  bool success = false;

  // flash LED
  pinMode(LED_BUILTIN, OUTPUT);
  for (int id = 0; id < 3; id++){
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
  }

  // init Serial
  Serial.begin(9600);
  delay(2000);

  // init filter
  arm_fir_decimate_instance_q15 fir_inst;
  q15_t StateQ15[AUDIO_BLOCK_SAMPLES + FILTER_TAP_NUM];
  int Mdec = 4;
  arm_fir_decimate_init_q15(&fir_inst, FILTER_TAP_NUM, Mdec,filter_taps,
                &StateQ15[0], AUDIO_BLOCK_SAMPLES);

  // create signal with 256 numbers
  for (int id=0; id < AUDIO_BLOCK_SAMPLES; id++){
    input[id] = id;
  }
  // print input
  Serial.println("Input:");
  for (int id=0; id < AUDIO_BLOCK_SAMPLES; id++){
    Serial.println(input[id]);
  }
  Serial.println();
  Serial.println();

  // calculate output signal
  arm_fir_decimate_fast_q15(&fir_inst, (q15_t *)input,
    (q15_t *)output, AUDIO_BLOCK_SAMPLES);

  // print output
  Serial.println("Output:");
  for (int id=0; id < AUDIO_BLOCK_SAMPLES/4; id++){
    Serial.println(output[id]);
  }


  success = true;

  // show success
  if (success){
    digitalWrite(LED_BUILTIN, HIGH);
    delay(2000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(2000);
  }
}

void loop() {
    // put your main code here, to run repeatedly:
}
