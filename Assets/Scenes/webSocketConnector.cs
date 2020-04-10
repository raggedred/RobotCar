using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Net.Sockets;
using System.IO;
using System;

public class webSocketConnector : MonoBehaviour
{


    internal Boolean socketReady = false;
    TcpClient mySocket;
    NetworkStream theStream;
    StreamWriter theWriter;
    StreamReader theReader;
    public String Host = "192.168.1.223"; // eg String Host = "192.168.1.1";
    public int Port = 5001; // eg Int32 Port = 5555;
    public bool start;

    String reader;

    void Start()
    {

    }

    void Update()
    {
        if (start)
        {
            setupSocket();
            //WriteSocket();
            CloseSocket();
            start = false;

        }
    }


    public void setupSocket()
    {                            // Socket setup here
        try
        {
            //            Host = "192.168.2.17";
            //            Port = 8000; 
            mySocket = new TcpClient(Host, Port);
            theStream = mySocket.GetStream();
            theWriter = new StreamWriter(theStream);
            theReader = new StreamReader(theStream);
            socketReady = true;
            //writeSocket("");
            Debug.Log("Connected!");
            socketReady = true;
        }
        catch (Exception e)
        {
            Debug.Log("Socket error:" + e);                // catch any exceptions
        }
    }

    public void WriteSocket(string theLine)
    {
        if (!socketReady)
            return;
        String foo = theLine + "\r\n";
        theWriter.Write(foo);
        theWriter.Flush();
    }
    public void WriteIntSocket(int theLine)
    {
        if (!socketReady)
            return;
        theWriter.Write(theLine);
        theWriter.Flush();
    }
    public void WriteCharSocket(char theLine)
    {
        if (!socketReady)
            return;
        theWriter.Write(theLine);
        theWriter.Flush();
    }

    public string ReadSocket()
    {
        if (!socketReady)
            return "";
        try
        {
            return theReader.ReadLine();
        }
        catch (Exception e)
        {
            Debug.Log("Error ReadSocket: " + e);
            return "";
        }
    }

    public void CloseSocket()
    {
        if (!socketReady)
            return;
        theWriter.Close();
        theReader.Close();
        mySocket.Close();
        socketReady = false;
    }
}