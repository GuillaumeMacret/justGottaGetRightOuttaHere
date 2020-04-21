using System;
using System.Collections;
using System.Collections.Generic;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using UnityEngine;

public class TCPClient
{
    private static TCPClient INSTANCE;
    public static TCPClient GetInstance()
    {
        if (INSTANCE == null) INSTANCE = new TCPClient();
        return INSTANCE;
    }

    public static bool socketConnected = false;
    public static void ConnectIfNotConnected()
    {
        if (socketConnected) return;
        ConnectToTcpServer();
    }

    #region private members 	
    private static TcpClient socketConnection;
    private static Thread clientReceiveThread;
    private const int BUFFER_SIZE = 50000;

    private const string SERVER_ADRESS = "norcisrasp.ddns.net";
    private const int SERVER_PORT = 1789;
    static IMessageHandler  m_MessageHandler = null;
    #endregion


    /// <summary> 	
    /// Sets the class that will handle incomming messages
    /// </summary> 	
    public static void SetMessageHandler(IMessageHandler handler)
    {
        m_MessageHandler = handler;
    }

    /// <summary> 	
    /// Setup socket connection. 	
    /// </summary> 	
    private static void ConnectToTcpServer()
    {
        Debug.Log("Trying to connect to " + SERVER_ADRESS + ":" + SERVER_PORT);
        try
        {
            clientReceiveThread = new Thread(new ThreadStart(ListenForData));
            clientReceiveThread.IsBackground = true;
            clientReceiveThread.Start();
            socketConnected = true;
            Debug.Log("Connected !");
        }
        catch (Exception e)
        {
            Debug.Log("On client connect exception " + e);
        }
    }
    /// <summary> 	
    /// Runs in background clientReceiveThread; Listens for incomming data. 	
    /// </summary>     
    private static void ListenForData()
    {
        try
        {
            socketConnection = new TcpClient(SERVER_ADRESS, SERVER_PORT);
            Byte[] bytes = new Byte[BUFFER_SIZE];
            while (true)
            {
                // Get a stream object for reading 				
                using (NetworkStream stream = socketConnection.GetStream())
                {
                    int length;
                    // Read incomming stream into byte arrary. 					
                    while ((length = stream.Read(bytes, 0, bytes.Length)) != 0)
                    {
                        var incommingData = new byte[length];
                        Array.Copy(bytes, 0, incommingData, 0, length);
                        // Convert byte array to string message. 						
                        string serverMessage = Encoding.ASCII.GetString(incommingData);
                        Debug.Log("server message received as: " + serverMessage);
                        if(m_MessageHandler != null)
                        {
                            m_MessageHandler.Handle(serverMessage.Remove(serverMessage.Length-1));
                        }
                    }
                }
            }
        }
        catch (SocketException socketException)
        {
            Debug.Log("Socket exception: " + socketException);
        }
    }
    /// <summary> 	
    /// Send message to server using socket connection. 	
    /// </summary> 	
    public static void SendMessage(string clientMessage)
    {
        if (socketConnection == null)
        {
            return;
        }
        try
        {
            // Get a stream object for writing. 			
            NetworkStream stream = socketConnection.GetStream();
            if (stream.CanWrite)
            {
                // Convert string message to byte array.                 
                byte[] clientMessageAsByteArray = Encoding.ASCII.GetBytes(clientMessage);
                // Write byte array to socketConnection stream.                 
                stream.Write(clientMessageAsByteArray, 0, clientMessageAsByteArray.Length);
                stream.Flush();
                Debug.Log("Client sent " + clientMessage);
            }
        }
        catch (SocketException socketException)
        {
            Debug.Log("Socket exception: " + socketException);
        }
    }
}