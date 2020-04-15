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
                var loadLevelJson = JSON.Parse(JSONString);
                JSONArray blocks = loadLevelJson["Blocks"].AsArray;
                JSONArray players = loadLevelJson["Players"].AsArray;
                model.LoadLevel(blocks, players);
                break;
            case "move":
                var moveJson = JSON.Parse(JSONString);
                model.MovePlayer(moveJson["Player"], moveJson["PosX"], moveJson["PosY"]);
                break;
            default:
                Debug.LogError("Can't handle this action : " + action.Action);
                break;
        }
    }
}
