# Directories
CLIENT_DIR = client
SERVER_DIR = server

# Targets
all:
	$(MAKE) -C $(CLIENT_DIR)
	$(MAKE) -C $(SERVER_DIR)

clean:
	$(MAKE) -C $(CLIENT_DIR) clean
	$(MAKE) -C $(SERVER_DIR) clean