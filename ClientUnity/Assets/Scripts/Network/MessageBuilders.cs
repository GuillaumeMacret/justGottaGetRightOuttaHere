using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MessageBuilders
{
    public static string BuildMovementMessage(string direction)
    {
        return "{\"Action\":\"move\",\"Direction\":\"" + direction + "\"}";
    }

    public static string BuildActionMessage()
    {
        return "{\"Action\":\"action\"}";
    }
}
