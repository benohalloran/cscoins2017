from Crypto.PublicKey import RSA
from Crypto.Hash import SHA256
from Crypto import Random
from Crypto.Signature import PKCS1_v1_5
from binascii import unhexlify





signature =  open('wallet.sig').read() 

pubkey_pem = open('rsa-public.pem').read() 
pubkey_der_fromfile = open('rsa-public.der', 'rb').read()
pub_key = RSA.importKey(pubkey_pem)
pub_key_der = RSA.importKey(pubkey_pem)
pubkey_der_exported = pub_key_der.exportKey(format='DER')

h = SHA256.new(pubkey_der_fromfile)
wallet_id = h.hexdigest()

print("wallet_id: " + wallet_id)

verifier = PKCS1_v1_5.new(pub_key)
if not verifier.verify(h, signature.decode('hex')):
    print('verify failed')
else:
    print('yay!')
