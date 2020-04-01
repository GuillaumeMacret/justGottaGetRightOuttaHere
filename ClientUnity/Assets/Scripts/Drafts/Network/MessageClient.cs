using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using System.Net.Sockets;
using System;

public class MessageClient
{
    private static MessageClient INSTANCE = null;

    public static MessageClient GetInstance()
    {
        if(INSTANCE == null)INSTANCE = new MessageClient();
        return INSTANCE;
    }

    static string m_serverAdress = "127.0.0.1";
    static int m_serverPort = 1789;

    static TcpListener m_tcpListener;
    static TcpClient tcpClient;

    public static void Connect()
    {
        try
        {
            tcpClient = new TcpClient(m_serverAdress, m_serverPort);
            Debug.Log("Client connected");
        }
        catch (SocketException e)
        {
            Debug.LogError("SocketException: " + e);
        }

    }

    public static void Send(string message)
    {
        Byte[] data = System.Text.Encoding.ASCII.GetBytes(message);
        NetworkStream stream = tcpClient.GetStream();
        Debug.Log("Sending: " + message);
        stream.Write(data, 0, data.Length);
        stream.Flush();

    }
}
