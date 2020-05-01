using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class UpdateServerAddr : MonoBehaviour
{
    private Text txt;
    private void Awake()
    {
        txt = GetComponent<Text>();
    }
    private void Update()
    {
        if(txt.text != "")
        {
            TCPClient.serverAddr = txt.text;
        }
    }
}
