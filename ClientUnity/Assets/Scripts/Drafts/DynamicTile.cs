using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Tilemaps;

public class DynamicTile : TileBase
{
    public SpriteRenderer spriteRenderer;
    public SpriteLoader spriteLoader;

    private int m_spriteId;
    
    public void ChangeSprite(int index)
    {
        m_spriteId = index;
        spriteRenderer.sprite = spriteLoader.getSprite(index);

    }
}
