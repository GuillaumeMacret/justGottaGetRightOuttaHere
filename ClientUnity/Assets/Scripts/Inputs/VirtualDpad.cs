using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class VirtualDpad : MonoBehaviour
{
    public Text directionText;
    private Touch theTouch;
    private Vector2 touchStartPosition, touchEndPosition;
    private string direction;

    private float m_InputCooldown = .25f;
    private float m_TimerCooldown = 0.0f;

    // Update is called once per frame
    void Update()
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
        if(m_TimerCooldown > 0)
        {
            m_TimerCooldown -= Time.deltaTime;
            //Debug.Log("Send is on cooldown " + m_TimerCooldown + " seconds left");
        }
        else
        {
            /* Create the message and send it */
            string messageToSend = "";
            if (direction != "" && direction != "Tapped")
            {
                messageToSend = MessageBuilders.BuildMovementMessage(direction);
                m_TimerCooldown = m_InputCooldown;
            }
            else if (direction == "Tapped")
            {
                messageToSend = MessageBuilders.BuildActionMessage();
                m_TimerCooldown = m_InputCooldown;
            }

            if (messageToSend != "")
            {
                TCPClient.SendMessage(messageToSend);
            }
        }

    }
}
