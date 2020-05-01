using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PingFade : MonoBehaviour
{
    private SpriteRenderer m_SpriteRenderer;
    private int m_FadeSpeed = 3;

    private void Awake()
    {
        m_SpriteRenderer = GetComponent<SpriteRenderer>();
    }
    // Update is called once per frame
    void Update()
    {
        if(m_SpriteRenderer.color.a <= 0)
        {
            Destroy(this.gameObject);
        }
        Color c = m_SpriteRenderer.color;
        c.a -= m_FadeSpeed / 255.0f;
        m_SpriteRenderer.color = c;
    }
}
