using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public interface IMessageHandler
{
    void Handle(string JSONString);
}
