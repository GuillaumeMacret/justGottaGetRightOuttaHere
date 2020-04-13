using UnityEngine;
using SimpleJSON;

public class GameBoardMessageHandler : MonoBehaviour, IMessageHandler
{
    public GameModel model;

    public void Handle(string JSONString)
    {
        ActionJson action = JsonUtility.FromJson<ActionJson>(JSONString);
        Debug.Log("Action recieved : " + action.Action);
        switch (action.Action)
        {
            case "loadLevel":
                var N = JSON.Parse(JSONString);
                JSONArray blocks = N["Blocks"].AsArray;
                model.LoadLevel(blocks.Count,blocks[0].Count,blocks);
                break;
            default:
                Debug.LogError("Can't handle this action : " + action.Action);
                break;

        }
    }
}
