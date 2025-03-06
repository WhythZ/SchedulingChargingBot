#ifndef _RESOURCE_MANAGER_H_
#define _RESOURCE_MANAGER_H_

#include <unordered_map>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "../Manager.hpp"

//ͼ����Դ����Ƶ��Դ��������Դ
enum class TextureResID
{
	Tileset,
	Vehicle,
	Robot,
	Battery
};

enum class SoundResID
{
	Pick_Battery
};

enum class FontResID
{
	Ipix,
	VonwaonBitmap12,
	VonwaonBitmap16
};

//ͼ����ࡢ�������ࡢ�������
typedef std::unordered_map<TextureResID, SDL_Texture*> TexturePool;
typedef std::unordered_map<SoundResID, Mix_Chunk*> SoundPool;
typedef std::unordered_map<FontResID, TTF_Font*> FontPool;

//��Դ������
class ResourceManager :public Manager<ResourceManager>
{
	friend class Manager<ResourceManager>;

private:
	TexturePool texturePool;              //ͼ���
	SoundPool soundPool;                  //��Ч��
	FontPool fontPool;                    //�����

public:
	bool LoadResource(SDL_Renderer*);

	const TexturePool& GetTexturePool();
	const SoundPool& GetSoundPool();
	const FontPool& GetFontPool();

private:
	ResourceManager() = default;
	~ResourceManager() = default;

	bool LoadTextureRes(SDL_Renderer*);
	bool LoadSoundRes();
	bool LoadFontRes();
};

#endif
