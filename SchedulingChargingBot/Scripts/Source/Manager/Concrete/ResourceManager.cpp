#include "../../../Header/Manager/Concrete/ResourceManager.h"

bool ResourceManager::LoadResource(SDL_Renderer* _renderer)
{
	if (!LoadTextureRes(_renderer)) return false;
	if (!LoadSoundRes()) return false;
	if (!LoadFontRes()) return false;

	return true;
}

const TexturePool& ResourceManager::GetTexturePool()
{
	return texturePool;
}

const SoundPool& ResourceManager::GetSoundPool()
{
	return soundPool;
}

const FontPool& ResourceManager::GetFontPool()
{
	return fontPool;
}

bool ResourceManager::LoadTextureRes(SDL_Renderer* _renderer)
{
	//��Ϊ�˴����������õ�SDL_Surface*�������Կ���ֱ��ʹ��IMG_LoadTexture()����ʡ�Խ����������һ
	//SDL_Surface* _imgSurface = IMG_Load("Assets/xx.jpg");                             //�������ص��ڴ棨�ڴ�ṹ�壩
	//SDL_Texture* _imgTexture = SDL_CreateTextureFromSurface(_renderer, _imgSurface);  //���ڴ���ص��Դ棨GPU�������ݣ�

	//��ͼƬ��Դ���ش洢��ͼ��أ�unordered_map���͵�������
	texturePool[TextureResID::Tileset] = IMG_LoadTexture(_renderer, "Assets/Sprites/Tileset.png");
	texturePool[TextureResID::Vehicle] = IMG_LoadTexture(_renderer, "Assets/Sprites/Vehicle.png");
	texturePool[TextureResID::Robot] = IMG_LoadTexture(_renderer, "Assets/Sprites/Robot.png");
	texturePool[TextureResID::Battery] = IMG_LoadTexture(_renderer, "Assets/Sprites/Battery.png");
	texturePool[TextureResID::CursorHover] = IMG_LoadTexture(_renderer, "Assets/Sprites/CursorHover.png");
	//texturePool[TextureResID::CursorHover] = IMG_LoadTexture(_renderer, "Assets/Sprites/CursorSelect.png");

	//���unordered_map���������м�ֵ�Ե�ֵ�Ƿ���Ч������auto���Զ������Ƶ�����Աsecond���ʵ��Ǽ�ֵ�Ե�ֵ
	for (const auto& _pair : texturePool)
		if (!_pair.second) return false;
	//�������ͷ��ؼ��سɹ�
	return true;
}

bool ResourceManager::LoadSoundRes()
{
	//���ļ��м�����Ч
	soundPool[SoundResID::Pick_Battery] = Mix_LoadWAV("Assets/Sounds/Pick_Battery.wav");

	//���unordered_map���������м�ֵ�Ե�ֵ�Ƿ���Ч������auto���Զ������Ƶ�����Աsecond���ʵ��Ǽ�ֵ�Ե�ֵ
	for (const auto& _pair : soundPool)
		if (!_pair.second) return false;
	//�������ͷ��ؼ��سɹ�
	return true;
}

bool ResourceManager::LoadFontRes()
{
	//���ļ��м�������
	fontPool[FontResID::Ipix] = TTF_OpenFont("Assets/Fonts/Ipix.ttf", 25);
	fontPool[FontResID::VonwaonBitmap12] = TTF_OpenFont("Assets/Fonts/VonwaonBitmap-12pxLite.ttf", 12);
	fontPool[FontResID::VonwaonBitmap16] = TTF_OpenFont("Assets/Fonts/VonwaonBitmap-16pxLite.ttf", 16);

	//���unordered_map���������м�ֵ�Ե�ֵ�Ƿ���Ч������auto���Զ������Ƶ�����Աsecond���ʵ��Ǽ�ֵ�Ե�ֵ
	for (const auto& _pair : fontPool)
		if (!_pair.second) return false;
	//�������ͷ��ؼ��سɹ�
	return true;
}