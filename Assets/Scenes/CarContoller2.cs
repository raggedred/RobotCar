using UnityEngine;
using UnityEngine.UI;
using System.Collections;

public class CarContoller2 : MonoBehaviour
{

	// PUBLIC
	public Text Info;
	public webSocketConnector driveController;
	private controllerData2 myObject;


	int i = 0;
	bool sent = false;
	void Start()
	{
		myObject = new controllerData2();
	}
	void Update()
	{


	}

	public void sendAction(int action)
	{
		int value = 255;
		bool received = false;
		//myObject.speed = 0;
		//myObject.direction = 0;
		//myObject.action = action;
		//string json = JsonUtility.ToJson(myObject);
		driveController.setupSocket();
		//driveController.WriteSocket(json);
		do
		{
			driveController.WriteIntSocket(action);
			Debug.Log("Sent: " + action);

			string read = driveController.ReadSocket();
			Debug.Log(read);

			if (read == "" || read == "255" || read == null)
			{
				received = false;
				Info.text = "Not Sent";
			}
			else
			{

				received = true;
				value = int.Parse(read);
				Info.text = "Sent";
			}

		} while (!received);



		driveController.CloseSocket();

	}

	public void sendAction(string action)
	{
		int value = 255;
		bool received = false;
		int count = 0;
		//myObject.speed = 0;
		//myObject.direction = 0;
		myObject.action = action;
		string json = JsonUtility.ToJson(myObject);
		driveController.setupSocket();
		driveController.WriteSocket(json);
		Debug.Log(json);
		/*
		do
		{
			char newChar = action.ToCharArray()[0];

			driveController.WriteCharSocket(newChar);
			Debug.Log("Sent: " + action);

			string read = driveController.ReadSocket();
			Debug.Log(read);
			int result = IntParseFast(read);
			if (result == 0 || result == 255 || result.Equals(null))
			{
				received = false;
				Info.text = "Not Sent";
			}
			else
			{

				received = true;
				value = int.Parse(read);
				Info.text = "Sent";
			}
			count++;

		} while (!received && count <= 10);

	*/
		string read = driveController.ReadSocket();
		Debug.Log(read);
		driveController.CloseSocket();

	}

	public static int IntParseFast(string value)
	{
		int result = 0;
		for (int i = 0; i < value.Length; i++)
		{
			char letter = value[i];
			result = 10 * result + (letter - 48);
		}
		return result;
	}

	[SerializeField]
	private class controllerData2
	{
		public string action;
	}
}

