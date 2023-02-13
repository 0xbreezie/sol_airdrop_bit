#include <solana_sdk.h>
#include <stdio.h>

#define NUM_RECIPIENTS 5

// Define the recipient data structure
struct Recipient {
  uint8_t pubkey[PUBKEY_SIZE];
  uint64_t amount;
};

// List of recipients
struct Recipient recipients[NUM_RECIPIENTS] = {
  {{0x01, 0x02, 0x03, ..., 0x20}, 1000},
  {{0x21, 0x22, 0x23, ..., 0x40}, 2000},
  {{0x41, 0x42, 0x43, ..., 0x60}, 3000},
  {{0x61, 0x62, 0x63, ..., 0x80}, 4000},
  {{0x81, 0x82, 0x83, ..., 0xA0}, 5000}
};

int main(void) {
  // Initialize the Solana SDK
  if (!solana_sdk_init()) {
    printf("Failed to initialize Solana SDK");
    return 1;
  }

  // Set the source wallet address and secret key
  uint8_t source_pubkey[PUBKEY_SIZE];
  uint8_t source_secret_key[SECRET_KEY_SIZE];
  solana_sdk_get_public_key(source_secret_key, source_pubkey);

  // Loop through the recipients list
  for (int i = 0; i < NUM_RECIPIENTS; i++) {
    struct Recipient recipient = recipients[i];

    // Create the Solana transaction
    SolTransaction transaction;
    solana_sdk_init_transaction(&transaction);
    solana_sdk_add_transfer_instruction(&transaction, source_pubkey, recipient.pubkey, recipient.amount);

    // Sign and submit the transaction
    if (solana_sdk_sign_and_submit_transaction(source_secret_key, &transaction) != 0) {
      printf("Failed to send tokens to recipient %d", i);
      continue;
    }

    printf("Successfully sent %d tokens to recipient %d", recipient.amount, i);
  }

  // Clean up the Solana SDK
  solana_sdk_free();

  return 0;
}
