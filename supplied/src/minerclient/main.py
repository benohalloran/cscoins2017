import sys
sys.path.append('..')
import MinerClient
import asyncio

keys_dir = ""
hostname = "ec2-54-186-71-144.us-west-2.compute.amazonaws.com"

if len(sys.argv) > 1:
    hostname = sys.argv[1]
    if len(sys.argv) > 2:
        keys_dir = sys.argv[2]

mc = MinerClient.MinerClient(keys_dir, hostname)
# mc.ssl = False
asyncio.get_event_loop().run_until_complete(mc.client_loop())
