using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class VirtualDpad : MonoBehaviour
{
    public GameModel model;

    private Touch theTouch;
    private Vector2 touchStartPosition, touchEndPosition;
    private string direction;

    public bool actionMode = true;
    private float m_cooldownAfterSwitch = .1f;
    private float m_cooldownTimer = 0f;

    private float m_InputCooldown = .1f;
    private float m_TimerCooldown = 0.0f;

    // Update is called once per frame
    void Update()
    {
        if (m_cooldownTimer > 0)
        {
            m_cooldownTimer -= Time.deltaTime;
        }
        else
        {
            /* Getting the touch */
            direction = "";
            if (Input.touchCount > 0)
            {
                theTouch = Input.GetTouch(0);

                if (theTouch.phase == TouchPhase.Began)
                {
                    touchStartPosition = theTouch.position;
                }

                else if (theTouch.phase == TouchPhase.Moved || theTouch.phase == TouchPhase.Ended)
                {
                    touchEndPosition = theTouch.position;

                    float x = touchEndPosition.x - touchStartPosition.x;
                    float y = touchEndPosition.y - touchStartPosition.y;

                    if (Mathf.Abs(x) == 0 && Mathf.Abs(y) == 0)
                    {
                        direction = "Tapped";
                    }

                    else if (Mathf.Abs(x) > Mathf.Abs(y))
                    {
                        direction = x > 0 ? "right" : "left";
                    }

                    else
                    {
                        direction = y > 0 ? "up" : "down";
                    }
                }
            }
            /* Decide if the send is on cooldown */
            if (m_TimerCooldown > 0)
            {
                m_TimerCooldown -= Time.deltaTime;
                //Debug.Log("Send is on cooldown " + m_TimerCooldown + " seconds left");
            }
            else
            {
                /* Create the message and send it */
                if (direction != "" && direction != "Tapped")
                {
                    SendMoveMessage(direction);
                    m_TimerCooldown = m_InputCooldown;
                }
                else if (direction == "Tapped")
                {
                    if (actionMode)
                    {
                        SendActionMessage();
                    }
                    else
                    {
                        SendPingMessage(Mathf.FloorToInt(touchEndPosition.x), Mathf.FloorToInt(touchEndPosition.y));
                    }
                    m_TimerCooldown = m_InputCooldown;
                }
            }
        }
    }
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

    public void SendPingMessage(int x,int y)
    {
        Vector2 convertedCoord = model.PixelPosToTilePos(x, y);
        string messageToSend = MessageBuilders.BuildPingMessage(Mathf.FloorToInt(convertedCoord.x), Mathf.FloorToInt(convertedCoord.y));
        TCPClient.SendMessage(messageToSend);
    }

    public void SendReturnMessage()
    {
        string messageToSend = MessageBuilders.BuildLobbyMessage();
        m_cooldownTimer = m_cooldownAfterSwitch;
        TCPClient.SendMessage(messageToSend);
    }

    public void SwitchToActionMode()
    {
        actionMode = true;
        m_cooldownTimer = m_cooldownAfterSwitch;
        Color c = model.actionButtonImage.color;
        c.a = 150/255.0f;
        model.actionButtonImage.color = c;
        c = model.pingButtonImage.color;
        c.a = 100/255.0f;
        model.pingButtonImage.color = c;
    }

    public void SwitchToPingMode()
    {
        actionMode = false;
        m_cooldownTimer = m_cooldownAfterSwitch;
        Color c = model.actionButtonImage.color;
        c.a = 100/255.0f;
        model.actionButtonImage.color = c;
        c = model.pingButtonImage.color;
        c.a = 150/255.0f;
        model.pingButtonImage.color = c;
    }
}
