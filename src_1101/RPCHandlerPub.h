int rpc_commond()
{
        std::cout << "Please enter a command" << std::endl;
        int rc;
        struct mosquitto *mosq;

        mosquitto_lib_init();

        mosq = mosquitto_new("publisher-test", true, NULL);

        rc = mosquitto_connect(mosq, "localhost", 1883, 60);
        if (rc != 0)
        {
                std::cout << "Cannot connect to broker " << rc << std::endl;
                mosquitto_destroy(mosq);
                return -1;
        }
        char line[128];
        // std::cin >> line;
        getchar();
        std::cin.getline(line, sizeof(line));
        const void *msg_pub = line;
        mosquitto_publish(mosq, NULL, "test/t1", sizeof(line),
                          msg_pub /*pointer to the data to send*/, 0, false);

        mosquitto_disconnect(mosq);
        mosquitto_destroy(mosq); //

        mosquitto_lib_cleanup();

        return 0;
}
