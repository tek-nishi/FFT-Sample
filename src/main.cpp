//
// FFTサンプル
//  44.1kHz 16bit mono のWAVファイルを再生しながら
//  波形とFFTを折れ線グラフで表示する
//

#include "lib/framework.hpp"
#include <complex>
#include <vector>
#include <cmath>
#include <unsupported/Eigen/FFT>


enum Window {
  WIDTH  = 800,
  HEIGHT = 400
};


// wavデータをfloat型に変換
std::vector<float> convertWavData(const Wav& wav_data, int offset, int sample_num) {
  // 取り出すサンプル数を決める
  // FIXME:量子化ビット数が16固定
  int sample_max_size = wav_data.size() / sizeof(short);
  int sample_size = std::min(sample_num, sample_max_size - offset);

  // TIPS:void* 経由で安全に型キャスト
  const void* ptr = wav_data.data();
  const short* pcm_data = static_cast<const short*>(ptr);

  // short→float
  std::vector<float> samples(sample_num, 0.0f);
  for (int i = 0; i < sample_size; ++i) {
    samples[i] = float(pcm_data[offset + i]) / 32768.0f;
  }

  return samples;
}


int main() {
  AppEnv env(Window::WIDTH, Window::HEIGHT);

  Wav wav_data("res/sample.wav");
  size_t sampling_rate = wav_data.sampleRate();
  
  Media bgm("res/sample.wav");
  bgm.play();
  
  while (env.isOpen()) {
    env.begin();

    if (bgm.isPlaying()) {
      // 再生位置(秒)を取得
      float t = bgm.currentTime();

      // 波形データのどの位置か割り出す
      int offset = t * sampling_rate;

      // 再生位置から適当なサンプル数の波形を取り出す
      std::vector<float> samples = convertWavData(wav_data, offset, 1024);

      // 取り出した波形を折れ線グラフで描画
      for (size_t i = 1; i < samples.size(); ++i) {
        float x1 = (float(Window::WIDTH) / samples.size()) * (i - 1) - (Window::WIDTH / 2);
        float x2 = (float(Window::WIDTH) / samples.size()) * i - (Window::WIDTH / 2);

        float y1 = float(Window::HEIGHT / 2) * samples[i - 1];
        float y2 = float(Window::HEIGHT / 2) * samples[i];
        
        drawLine(x1, y1, x2, y2, 2, Color(1, 0, 0));
      }

      // FFT実行
      Eigen::FFT<float> fft;
      std::vector<std::complex<float> > freq;
      fft.fwd(freq, samples);

      // FFT解析結果を折れ線グラフで表示
      //  左右対称な結果が得られるので、半分だけ表示している
      size_t freq_size = freq.size() / 2;
      for (size_t i = 1; i < freq_size; ++i) {
        float x1 = (float(Window::WIDTH) / freq_size) * (i - 1) - (Window::WIDTH / 2);
        float x2 = (float(Window::WIDTH) / freq_size) * i - (Window::WIDTH / 2);

        std::complex<float> a = freq[i - 1];
        std::complex<float> b = freq[i];
        
        float y1 = std::sqrt(a.real() * a.real() + a.imag() * a.imag()) * 2.5f - (Window::HEIGHT / 2 - 5);
        float y2 = std::sqrt(b.real() * b.real() + b.imag() * b.imag()) * 2.5f - (Window::HEIGHT / 2 - 5);
        
        drawLine(x1, y1, x2, y2, 2, Color(0, 0, 1));
      }      
    }
  
    env.end();
  }
}
