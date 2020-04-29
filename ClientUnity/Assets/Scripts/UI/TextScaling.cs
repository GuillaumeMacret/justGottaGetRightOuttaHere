using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class TextScaling : MonoBehaviour
{
    public int size;
    // Start is called before the first frame update
    void Awake()
    {
        GetComponent<Text>().fontSize = Screen.width / size;
    }
    private void Update()
    {
        GetComponent<Text>().fontSize =(int) (Screen.width / (float) size);
    }
}
