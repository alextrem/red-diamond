/*
    Red-Diamond - Copyright (C) 2018 Alexander Geissler

    Licensed under the GNU General Public License, Version 3 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        https://www.gnu.org/licenses/gpl-3.0.en.html

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "ch.h"
#include "hal.h"
#include "shell_commands.h"

static void cmd_led(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)argv;
  if(argc > 0) {
    chprintf(chp, "Usage: led\r\n");
    return;
  }
  chprintf(chp, "toggling leds\r\n");
  palTogglePad(GPIOD, 12);
  palTogglePad(GPIOD, 13);
  palTogglePad(GPIOD, 14);
  palTogglePad(GPIOD, 15);
}

static void cmd_mad(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void) argv;
  if (argc > 0) {
    chprintf(chp, "Usage: mad\r\n");
    return;
  }
  chprintf(chp, "Libmad: %s, %s, %s\r\n", __FILE__, __DATE__, __TIME__);
  chprintf(chp, "%s\r\n", mad_version);
  chprintf(chp, "%s\r\n", mad_copyright);
  chprintf(chp, "%s\r\n", mad_author);
  chprintf(chp, "%s\r\n", mad_build);
}

static void cmd_adc(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void) argv;
  chprintf(chp, "Temperature: %d \r\n", temperature );
  if (argc > 0) {
    chprintf(chp, "Usage: adc\r\n");
    return;
  }
}

static void cmd_codec(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void) argv;
  if (argc > 2) {
    chprintf(chp, "Usage: codec [command]\r\n");
    return;
  }

  if (strcmp("id", argv[0]) == 0) {
    uint8_t id = Codec_GetID(&I2CD1);
    chprintf(chp, "ID: 0x%x\r\n", id);
  }
  else if (strcmp("mute", argv[0]) == 0) {
    Codec_Mute(&I2CD1, all);
    chprintf(chp, "Muting outputs\r\n");
  }
  else if (strcmp("beep", argv[0]) == 0) {
    Codec_BeepGenerator(&I2CD1);
    chprintf(chp, "Setting Beep frequency to 1kHz\r\n");
  }
  else if (strcmp("vol", argv[0]) == 0) {
    uint32_t volume = atoi(argv[1]);
    if (volume < 256) {
      Codec_VolumeCtrl(&I2CD1, all, volume);
      chprintf(chp, "Set volume to %d\r\n", volume);
    }
  }
}

static void cmd_dac(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void) argv;
  if (argc > 0) {
    chprintf(chp, "Usage: dac [command]\r\n");
    return;
  }
}

static const ShellCommand commands[] = {
  {"led", cmd_led},
  {"mad", cmd_mad},
  {"adc", cmd_adc},
  {"codec", cmd_codec},
  {"dac", cmd_dac},
  {"dir", cmd_dir},
  {NULL, NULL}
};

