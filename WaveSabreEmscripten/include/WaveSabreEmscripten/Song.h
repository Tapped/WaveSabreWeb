#include <WaveSabreCore.h>
#include <WaveSabreEmscripten/SongRenderer.h>

WaveSabreCore::Device *SongFactory(SongRenderer::DeviceId id)
{
	switch (id)
	{
	case SongRenderer::DeviceId::Falcon: return new WaveSabreCore::Falcon();
	case SongRenderer::DeviceId::Echo: return new WaveSabreCore::Echo();
	}
	return nullptr;
}

const unsigned char SongBlob[] =
{
	0x78, 0x00, 0x00, 0x00, 0x44, 0xac, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x20, 0x40, 0x02, 0x00, 0x00, 0x00,
	0x00, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00,
	0x00, 0x48, 0xe1, 0x1a, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x80, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x3f, 0xae, 0x47, 0xe1, 0x3e, 0x00, 0x00, 0x00,
	0x00, 0xff, 0x99, 0x93, 0x3e, 0x20, 0x85, 0x6b, 0x3e, 0x0a,
	0xd7, 0xa3, 0x3e, 0xcd, 0xcc, 0x4c, 0x3f, 0x89, 0x88, 0x08,
	0x3e, 0x29, 0x5c, 0x0f, 0x3e, 0x00, 0x00, 0x00, 0x3f, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x6a, 0xb4, 0xe7, 0x3c, 0x00,
	0x00, 0x00, 0x3f, 0x0a, 0xd7, 0x23, 0x3c, 0x00, 0x00, 0x00,
	0x3f, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x84, 0x00, 0x00, 0x00, 0x06, 0x28, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x40, 0x3e, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x80, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x31, 0xdf, 0x7f, 0x3f, 0x00, 0x00, 0x00, 0x3f,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x28, 0x00,
	0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x49, 0x64, 0xc4, 0x0a, 0x00, 0x00,
	0xc9, 0xc4, 0x0a, 0x00, 0x00, 0x4b, 0x64, 0xc4, 0x0a, 0x00,
	0x00, 0xcb, 0x89, 0x15, 0x00, 0x00, 0x50, 0x64, 0xc4, 0x0a,
	0x00, 0x00, 0xd0, 0xc4, 0x0a, 0x00, 0x00, 0x55, 0x64, 0xc5,
	0x0a, 0x00, 0x00, 0xd5, 0x88, 0x15, 0x00, 0x00, 0x47, 0x64,
	0xc4, 0x0a, 0x00, 0x00, 0xc7, 0x00, 0x00, 0x00, 0x00, 0x4b,
	0x64, 0xc5, 0x0a, 0x00, 0x00, 0xcb, 0x88, 0x15, 0x00, 0x00,
	0x53, 0x64, 0xc4, 0x0a, 0x00, 0x00, 0xd3, 0x00, 0x00, 0x00,
	0x00, 0x52, 0x64, 0xc5, 0x0a, 0x00, 0x00, 0xd2, 0x00, 0x00,
	0x00, 0x00, 0x33, 0x64, 0xc4, 0x0a, 0x00, 0x00, 0xb3, 0xc4,
	0x0a, 0x00, 0x00, 0x3a, 0x64, 0xc4, 0x0a, 0x00, 0x00, 0xba,
	0xc5, 0x0a, 0x00, 0x00, 0x44, 0x64, 0xc4, 0x0a, 0x00, 0x00,
	0xc4, 0xc4, 0x0a, 0x00, 0x00, 0x42, 0x64, 0xc4, 0x0a, 0x00,
	0x00, 0xc2, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x80, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x80, 0x3f, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, 0x00, 0x00,
	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

SongRenderer::Song Song = {
	SongFactory,
	SongBlob
};
