using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class NetworkRunner : MonoBehaviour
{
    public TextElementMessageHandler handler;
    // Start is called before the first frame update
    void Start()
    {
        TCPClient.ConnectIfNotConnected();
        TCPClient.SetMessageHandler(handler);
    }

    // Update is called once per frame
    void Update()
    {
    }
}
