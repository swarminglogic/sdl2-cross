#include <audio/FliteUtil.h>

#include <audio/SoundChunk.h>
#include <audio/flite.h>
#include <util/Log.h>


FliteUtil::FliteUtil()
{
}


FliteUtil::~FliteUtil()
{
}


sdl::SoundPtr FliteUtil::textToSpeech(const std::string& text)
{
  Log log("textToSpeech");

  // Registres voice with flite tts engine.
  static cst_voice* voice = register_cmu_us_rms(NULL);
  // static cst_voice* voice = register_cmu_us_awb(NULL);

  cst_utterance *u = flite_synth_text(text.c_str(), voice);
  cst_wave *wave = utt_wave(u);

  // Number of bytes in WAV RIFF header, plus space for samples
  const int waveDataByteSize = (4 + 4 + 4 + 4 + 4 + 2 + 2 +
                                4 + 4 + 2 + 2 + 4 + 4 +
                                (int)(sizeof(short) *
                                      cst_wave_num_channels(wave) *
                                      cst_wave_num_samples(wave)));

  // Allocating data for the SDL_RWops to write to
  std::unique_ptr<Uint8[]> waveData(new Uint8[waveDataByteSize]);

  // SDL Pointer To Memory
  SDL_RWops *rw = SDL_RWFromMem(waveData.get(), waveDataByteSize);
  bool isOk = writeWaveData(wave, rw);
  delete_utterance(u);
  if (!isOk) {
    log.e("Failed to write WAV data");
  }

  // Load wav data (with WAV header), and let SDL_mixer do conversion
  // to match output format (sample rate, bit depth, stereo).
  sdl::SoundPtr snd(Mix_LoadWAV_RW(rw, 1));

  // TODO swarminglogic, 2014-08-07: Change flite library to create
  // WAV data that matches the output channel so no conversion is necessary.
  // flite output data can then be loaded with Mix_QuickLoad_WAV
  // Also opens up for streaming playback.

  return std::move(snd);
}


bool FliteUtil::writeWaveData(const cst_wave *w, SDL_RWops* out)
{
    const char* info = "RIFF";
    SDL_RWwrite(out, (const void*)info, 4, 1);
    int num_bytes = ((cst_wave_num_samples(w) *
                      cst_wave_num_channels(w) *
                      (int)sizeof(short))) + (8 + 16 + 12);

    // num bytes in whole file
    SDL_RWwrite(out, &num_bytes, 4, 1);

    info = "WAVE";
    SDL_RWwrite(out, info, 1, 4);
    info = "fmt ";
    SDL_RWwrite(out, info, 1, 4);

    // size of header
    num_bytes = 16;
    SDL_RWwrite(out, &num_bytes, 4, 1);

    short d_short = RIFF_FORMAT_PCM;
    SDL_RWwrite(out, &d_short, 2, 1);

    // number of channels
    d_short = (short)cst_wave_num_channels(w);
    SDL_RWwrite(out, &d_short, 2, 1);

    // sample rate
    int d_int = cst_wave_sample_rate(w);
    SDL_RWwrite(out, &d_int, 4, 1);

    // average bytes per second
    d_int = (cst_wave_sample_rate(w) *
             cst_wave_num_channels(w) *
             (int)sizeof(short));
    SDL_RWwrite(out, &d_int, 4, 1);

    // block align
    d_short = (short)(cst_wave_num_channels(w) * sizeof(short));
    SDL_RWwrite(out, &d_short, 2, 1);

    // bits per sample
    d_short = 2 * 8;
    SDL_RWwrite(out, &d_short, 2, 1);

    info = "data";
    SDL_RWwrite(out, info, 1, 4);

    // bytes in data
    d_int = (cst_wave_num_channels(w) *
             cst_wave_num_samples(w) *
             (int)sizeof(short));
    SDL_RWwrite(out, &d_int, 4, 1);

    int n = SDL_RWwrite(out, cst_wave_samples(w), sizeof(short),
                        cst_wave_num_channels(w) * cst_wave_num_samples(w));

    SDL_RWseek(out, 0, RW_SEEK_SET);
    return n == (cst_wave_num_channels(w) * cst_wave_num_samples(w));
  }
