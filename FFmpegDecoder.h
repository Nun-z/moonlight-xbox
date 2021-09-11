#pragma once

#include "Common/DeviceResources.h"
#include<queue>
#include <FramePacer.h>


extern "C" {
#include <Limelight.h>
#include <libavcodec/avcodec.h>
}
#define MAX_BUFFER 1024 * 1024
namespace moonlight_xbox_dx
{
	
	class FFMpegDecoder {
	public: 
		FFMpegDecoder(std::shared_ptr<DX::DeviceResources> r, FramePacer *pacer) {
			resources = r;
			this->pacer = pacer;
		};
		int Init(int videoFormat, int width, int height, int redrawRate, void* context, int drFlags);
		void Start();
		void Stop();
		void Cleanup();
		int SubmitDU(PDECODE_UNIT decodeUnit);
		int GetFrame();
		static FFMpegDecoder* getInstance();
		static DECODER_RENDERER_CALLBACKS getDecoder();
		static FFMpegDecoder* createDecoderInstance(std::shared_ptr<DX::DeviceResources> resources, FramePacer *pacer);
		FramePacer *pacer;
	private:
		int Decode(unsigned char* indata, int inlen);
		AVPacket pkt;
		AVCodec* decoder;
		AVCodecContext* decoder_ctx;
		unsigned char* ffmpeg_buffer;
		int dec_frames_cnt;
		AVFrame** dec_frames;
		AVFrame** ready_frames;
		int next_frame, current_frame;
		std::shared_ptr<DX::DeviceResources> resources;
		Microsoft::WRL::ComPtr<ID3D11Device1> ffmpegDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> ffmpegDeviceContext;
		ID3D11Texture2D* ffmpegTexture;
	};
}