# Protocols

## Pregame (login and create)

### Give game list (server)
```JSON
{
    "Action":"gameList",
    "Games":[
        "Game":{
            "id":[gameId],
            "nbPlayers":[0|1|2|3|4],
        },
        "Game":{
            ...
        }
    ]
}
```

### Create a game (client)
```JSON
{
    "Action":"createGame"
}
```
Response from server :
```JSON
{
    "MapList": ["level1", "level2", ...]
}
```


### Ask to join a game (client)
```JSON
{
  "Action":"joinGame",
  "GameId":[id]
}
```
Responses to join a game (server)
```JSON
{
  "Action":"joinedGame",
  "GameId":[id],
  "Players":[id, id, ...],
  "PlayerId": id,
  "Map": "name"
}
```
```JSON
{
  "Action":"cantJoinGame",
  "GameId":[id],
  "MoreInfo":["gameFull"|"communicationError"|"gameDoesNotExist"|...]
}
```

### Ask to change player's role (client)

```JSON
{
    "Action":"roleChange",
    "RoleId":[id]
}
```

Responses from server (broadcast)
```JSON
{
  "Action":"roleChange",
  "PlayerId":[id],
  "RoleId":[id]
}
```
OSEF
```JSON
{
  "Action":"cantChangeRole",
  "MoreInfo":[...]
}
```

### Ask Change Map (leader)
```JSON
{
    "Action":"changeMap",
    "Map": "name"
}
```
Responses from server
```JSON
{
    "Action":"changedMap",
    "Map": "name"
}
```

### Ask Start game (client)(leader)

```JSON
{
    "Action":"startGame"
}
```

Responses from server
```JSON
{
  "Action":"loadLevel",
  "Name":[name],
  "Blocks":[[1,0,4,3,1,1,1,..],[...]],
  "Objects":[{xPos:[val],yPos:[val],value:[val]},...],
  "Players":[{xPos:0,yPos:0},{xPos:0,yPos:0},{xPos:0,yPos:0},{xPos:0,yPos:0}]
}
```

```JSON
{
  "Action":"cantStartGame",
  "MoreInfo":["Not enough players"|"Dup role"|...]
}
```

## Ingame

### Ask for a movement (client)

```JSON
{
    "Action":"move",
    "Direction":"up|down|left|right"
}
```

### Send a player movement (server)

```JSON
{
    "Action":"move",
    "PosX":[val],
    "PosY":[val],
    "Player":[id],
    "Changes":[{xPos:[val],yPos:[val],value:[val]},{xPos:[val],yPos:[val],value:[val]}]
}
```

### Action (player) (if appropriate)

```JSON
{
    "Action":"action"
}
```

Response from server if action allowed (new map status)

```JSON
{
    "Action":"action",
    "Changes":[{xPos:[val],yPos:[val],value:[val]},{xPos:[val],yPos:[val],value:[val]}]
}
```

### Players wons (server)

```JSON
{
    "Action":"win"
}
```

### Goto next level (client)(leader)

```JSON
{
    "Action":"nextLevel"
}
```

Response from server : load level (see "Ask Start game" response)

### Quit game

TODO

Response from server (broadcast)
```json
{
    "Action":"leaveGame",
    "Player":[id]
}
```
### Restart level

TODO
