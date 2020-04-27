using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ButtonInput : MonoBehaviour
{
    public void SendMoveMessage(string direction)
    {
        string messageToSend = MessageBuilders.BuildMovementMessage(direction);
        TCPClient.SendMessage(messageToSend);
    }

    public void SendActionMessage()
    {
        string messageToSend = MessageBuilders.BuildActionMessage();
        TCPClient.SendMessage(messageToSend);
    }

    public void SendReturnMessage()
    {
        string messageToSend = MessageBuilders.BuildLobbyMessage();
        TCPClient.SendMessage(messageToSend);
    }
}
