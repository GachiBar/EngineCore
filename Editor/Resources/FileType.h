#pragma once

enum FileType
{
    PlainText,
    Directory,
    Scene,
    Prefab,
    Mesh,
    Texture,
    Material,
    Meta,
    Other
};

// Name - just filename without extension
// Icon - for now use basic
// Types:
// - Plain text (first stage)
// - Mesh
// - Sprite
// - Scene
// - Prefab (maybe)
// - Directory
// Some serializable custom data