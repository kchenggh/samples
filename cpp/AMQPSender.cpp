//  Disclaimers
//  NO WARRANTY FOR SOURCE CODE
//  ALL SOURCE CODE ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED.

#include "proton/message.h"
#include "proton/messenger.h"

#include <getopt.h>
#include <proton/util.h>
#include <sys/time.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>

struct device {
	int deviceID = 123;
	template <class Archive>
	void serialize(Archive & ar)
	{
		ar(cereal::make_nvp("device ID", deviceID));
	}
};

#define check(messenger)                                                     \
  {                                                                          \
    if(pn_messenger_errno(messenger))                                        \
    {                                                                        \
      printf("check\n");                                                     \
      logerr(__FILE__, __LINE__, pn_error_text(pn_messenger_error(messenger))); \
    }                                                                        \
  }

void logerr(const char *file, int line, const char *message)
{
	fprintf(stderr, "%s:%i: %s\n", file, line, message);
	exit(1);
}

int sendMessage(pn_messenger_t * messenger) {
	char * address = (char *) "amqp_address";
	device dev;
	std::stringstream buffer;
	{
		cereal::JSONOutputArchive ar(buffer);
		dev.serialize(ar);
	}
	std::cout << buffer.str() << std::endl;
	char *szJsonMsg = new char[buffer.str().length() + 1];
	strcpy(szJsonMsg, buffer.str().c_str());

	pn_message_t * message;
	pn_data_t * body;
	message = pn_message();

	pn_message_set_address(message, address);
	pn_message_set_content_type(message, (char*) "application/octect-stream");
	pn_message_set_inferred(message, true);

	body = pn_message_body(message);
	pn_data_put_binary(body, pn_bytes(strlen(szJsonMsg), szJsonMsg));

	pn_messenger_put(messenger, message);
	check(messenger);
	pn_messenger_send(messenger, 1);
	check(messenger);

	pn_message_free(message);
	return 0;
}

int main(int argc, char** argv) {
	printf("Press Ctrl-C to stop the sender process\n");

	pn_messenger_t *messenger = pn_messenger(NULL);
	pn_messenger_set_outgoing_window(messenger, 1);
	pn_messenger_start(messenger);

	while (true) {
		sendMessage(messenger);
		sleep(10);
	}

	// release messenger resources
	pn_messenger_stop(messenger);
	pn_messenger_free(messenger);

	return 0;
}
