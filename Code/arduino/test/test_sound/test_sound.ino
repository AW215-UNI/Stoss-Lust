/*
 * Copyright 2018 Code and Make (codeandmake.com)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*
 * A simple project that demonstrates how to make music using an Arduino.
 * 
 * This code accompanies the following tutorial: https://youtu.be/Z1YvIFUIhLs
 */

// Pin to which an 8 Ohm speaker is connected (use a 150 - 220 Ohm resistor)
#define speakerPin D1

// Tempo (beats per minute)
#define bpm 96

// Gap between notes. This is expressed as percentage of time between 2 beats.
#define noteGapPercentage 10

/*
 * 2D array containing the notes to be played
 * A note comprises of two values:
 *  * The first value determines the frequency of the note.
 *    This is expressed as the number of a key on an 88-key piano (1 - 88)
 *    A number outside this range can be used (e.g. 0) to create a gap 
 *  * The second value determines the duration:
 *    * 1 represents a whole note (spans 4 beats)
 *    * 2 represents a half note (spans 2 beats)
 *    * 4 represents a quarter note (spans 1 beat)
 *    * 8 represents an eighth note (spans 080.5 beat)
 *      etc.
 */
uint8_t notes[][2] = {
   {30,4}, {32,8}, {60,4}, {80,1}, {30,4}
};

// Time between two beats in microseconds (equal to length of a quarter note)
#define beatDuration (60.0 / bpm) * 1000000L

// Time of the gap between two notes in microseconds
#define noteGap beatDuration * (noteGapPercentage / 100.0)

void setup() {
  // Set the speakerPin as an output
  pinMode(speakerPin, OUTPUT);

  // Iterate over the notes array
  for(int i = 0; i < (sizeof(notes) / sizeof(*notes)); i++) {
    // pass the key number and note type
    playNote(notes[i][0], notes[i][1]);
  }
}

/*
 * Plays an individual note.
 * 
 * keyNumber - The key number (1 - 88)
 * noteType - The note type (1, 2, 4, 8, etc.)
 */
void playNote(uint8_t keyNumber, uint8_t noteType) {
  long halfPeriod = getPeriodForKey(keyNumber) / 2;
  long noteDuration = beatDuration * (4.0 / noteType);
  long elapsed = 0;

  // While we have a note to play
  while(halfPeriod > 0 && elapsed < (noteDuration - noteGap)) {
    // Set speakerPin high for half of the period
    digitalWrite(speakerPin, HIGH);
    wait(halfPeriod);

    // Set speakerPin low for half of the period
    digitalWrite(speakerPin, LOW);
    wait(halfPeriod);

    // Update the amount of time that has elapsed
    elapsed += halfPeriod * 2;
  }

  /*
   * Gap between notes. Calculated using 'elapsed' to minimise timing errors
   * and ensure that the correct gap occurs whenever getPeriodForKey() returns
   * zero. 
   */
  wait(noteDuration - elapsed);
}

/*
 * Returns the period for a key or zero for key numbers outside the range of 1 -
 * 88.
 * 
 * keyNumber - The key number (1 - 88)
 */
long getPeriodForKey(uint8_t keyNumber) {
  // If the key is between 1 and 88
  if(keyNumber >= 1 && keyNumber <= 88) {
    // Return the period (one second divided by the frequency of the key)
    return 1000000L / (pow(2.0, (keyNumber - 49.0) / 12.0) * 440.0);
  }

  // Otherwise return zero
  return 0;
}

/*
 * Delay for a number of microseconds. This is necessary because
 * delayMicroseconds() has an upper limit.
 * 
 * us - The delay in microseconds
 */
void wait(long us) {
  // First delay for the number of whole milliseconds using delay()
  delay(us / 1000);
  // Then delay for the remainder of microseconds using delayMicroseconds()
  delayMicroseconds(us % 1000);
}

void loop() {
  // Not used. Music will play once.
}