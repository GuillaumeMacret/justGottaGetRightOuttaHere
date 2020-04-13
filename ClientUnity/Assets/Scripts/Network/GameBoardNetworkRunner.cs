using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameBoardNetworkRunner : MonoBehaviour
{
    public GameBoardMessageHandler handler;
    // Start is called before the first frame update
    void Start()
    {
        TCPClient.ConnectIfNotConnected();
        TCPClient.SetMessageHandler(handler);
    }
}
