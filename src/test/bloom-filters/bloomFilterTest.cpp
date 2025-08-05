#include "bloomFilterTest.h"

void testNumberOfHashFunctions() {
    assert(BLOOM_FILTER_NUM_HASH_FUNCTIONS == 6);
}

void testCalculateBitmapSizeForFirstLevel() {
	BloomFilter bf(1);
	assert(bf.getBitmapSize() == 1048576);
}

void testCalculateBitmapSizePastFirstLevel() {
	BloomFilter bf(2);
	assert(bf.getBitmapSize() == 2097152);

	BloomFilter bf2(3);
	assert(bf2.getBitmapSize() == 4194304);
}

void testInsertKey() {
	int level = 3;
	BloomFilter bf(level);
	uint64_t key = 123456789;

	bf.insertKey(key);

	char *bitmap = bf.getBitmap();

	// verify at least 1 bit is set
	bool bitSet = false;
	for (uint64_t i = 0; i < bf.getBitmapSize(); i++) {
		if (bitmap[i] == 1) {
			bitSet = true;
			break;
		}
	}
	assert(bitSet);

	// verify the correct bits are set
	uint64_t hashValue[2];
	BloomFilter::hash(key, hashValue);
	for (int i = 0; i < BLOOM_FILTER_NUM_HASH_FUNCTIONS; i++) {
		uint64_t hash = (hashValue[0] + i * hashValue[1]) % bf.getBitmapSize();
		assert(bitmap[hash] == 1);
	}

	uint64_t key2 = 987654321;
	bf.insertKey(key2);
	// verify that the bits for key1 and key2 are both set
	BloomFilter::hash(key, hashValue);
	for (int i = 0; i < BLOOM_FILTER_NUM_HASH_FUNCTIONS; i++) {
		uint64_t hash = (hashValue[0] + i * hashValue[1]) % bf.getBitmapSize();
		assert(bitmap[hash] == 1);
	}
	BloomFilter::hash(key2, hashValue);
	for (int i = 0; i < BLOOM_FILTER_NUM_HASH_FUNCTIONS; i++) {
		uint64_t hash = (hashValue[0] + i * hashValue[1]) % bf.getBitmapSize();
		assert(bitmap[hash] == 1);
	}
}

void testGetKeyBitLocationForHashFunction() {
	int bfPageNumbers[BLOOM_FILTER_NUM_HASH_FUNCTIONS] = {0};
	int bfPageOffsets[BLOOM_FILTER_NUM_HASH_FUNCTIONS] = {0};

	int level = 3;
	int bfPageNumber, bfPageOffset;

	uint64_t key = 123456789;

	// set up bloom filter
	BloomFilter bf(level);
	bf.insertKey(key);
	for (int i = 0; i < BLOOM_FILTER_NUM_HASH_FUNCTIONS; i++) {
		uint64_t hashValue[2];
		BloomFilter::hash(123456789, hashValue);
		uint64_t hash = (hashValue[0] + i * hashValue[1]) % bf.getBitmapSize();

		bfPageNumbers[i] = hash / PAGE_SIZE;
		bfPageOffsets[i] = hash % PAGE_SIZE;
	}

	// sanity check for page numbers and offsets
	for (int hashFunctionNumber = 1; hashFunctionNumber <= BLOOM_FILTER_NUM_HASH_FUNCTIONS; hashFunctionNumber++) {
		BloomFilter::getKeyBitLocationForHashFunction(key, level, hashFunctionNumber, bfPageNumber, bfPageOffset);
		assert(bfPageNumber == bfPageNumbers[hashFunctionNumber - 1]);
		assert(bfPageOffset == bfPageOffsets[hashFunctionNumber - 1]);
		assert(bf.getBitmap()[bfPageNumber * PAGE_SIZE + bfPageOffset] == 1);
	}

	// test consistency with other random keys
    std::vector<uint64_t> keys = {123, 456789, 112233, 445566778899};
    for (uint64_t key : keys) {
        bf.insertKey(key);
        for (int hashFunctionNumber = 1; hashFunctionNumber <= BLOOM_FILTER_NUM_HASH_FUNCTIONS; hashFunctionNumber++) {
            BloomFilter::getKeyBitLocationForHashFunction(key, level, hashFunctionNumber, bfPageNumber, bfPageOffset);

            assert(bfPageNumber >= 0 && bfPageNumber < bf.getBitmapSize() / PAGE_SIZE);
            assert(bfPageOffset >= 0 && bfPageOffset < PAGE_SIZE);
            assert(bf.getBitmap()[bfPageNumber * PAGE_SIZE + bfPageOffset] == 1);
        }
    }

    // test with random levels
    for (int testLevel = 1; testLevel <= 5; testLevel++) {
        BloomFilter bfLevelsTest(testLevel);

        bfLevelsTest.insertKey(key);
        for (int hashFunctionNumber = 1; hashFunctionNumber <= BLOOM_FILTER_NUM_HASH_FUNCTIONS; hashFunctionNumber++) {
            BloomFilter::getKeyBitLocationForHashFunction(key, testLevel, hashFunctionNumber, bfPageNumber, bfPageOffset);

            assert(bfPageNumber >= 0 && bfPageNumber < bfLevelsTest.getBitmapSize() / PAGE_SIZE);
            assert(bfPageOffset >= 0 && bfPageOffset < PAGE_SIZE);
            assert(bfLevelsTest.getBitmap()[bfPageNumber * PAGE_SIZE + bfPageOffset] == 1);
        }
    }
}

void bloomFilterTestAll() {
	cout << "Running Bloom Filter tests..." << endl;

    cout << "Running testNumberOfHashFunctions..." << endl;
    testNumberOfHashFunctions();
    cout << "testNumberOfHashFunctions passed" << endl;

    cout << "Running testCalculateBitmapSizeForFirstLevel..." << endl;
	testCalculateBitmapSizeForFirstLevel();
	cout << "testCalculateBitmapSizeForFirstLevel passed" << endl;
    cout << "Running testCalculateBitmapSizePastFirstLevel..." << endl;
	testCalculateBitmapSizePastFirstLevel();
	cout << "testCalculateBitmapSizePastFirstLevel passed" << endl;

    cout << "Running testInsertKey..." << endl;
	testInsertKey();
	cout << "testInsertKey passed" << endl;
    
    cout << "Running testGetKeyBitLocationForHashFunction..." << endl;
	testGetKeyBitLocationForHashFunction();
	cout << "testGetKeyBitLocationForHashFunction passed" << endl;

	cout << endl << "All Bloom Filter tests passed!" << endl;
}