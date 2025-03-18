Here's an updated version of the README that includes the note about `ctest`:

---

# Integration Test Data Setup

This directory contains the integration tests for the **obs2ioda** project. To run these tests, you must first download and prepare the required test data.

## Step 1: Download the Data

Download the integration test data from the following Google Drive link:

 [Integration Test Data](https://drive.google.com/file/d/1S957HOcQOPErQ_qGO_dySWOnKXUaUrOS/view)

This will download a file named `data.tar.gz`.

## Step 2: Decompress the Data

After downloading, decompress the file using:

```bash
tar -xvf data.tar.gz
```

This will create a directory named `data`.

## Step 3: Move Data to the Correct Location

Move the `data` directory into `test/integration/` under the project root (`<ROOT>`), where `<ROOT>` is the root directory of the **obs2ioda** project.

For example, if your project is located at `/path/to/obs2ioda`, run:

```bash
mv data /path/to/obs2ioda/test/integration/
```

The directory structure should look like:

```
obs2ioda/
├── src/
├── test/
│   ├── unit/
│   ├── integration/
│   │   └── data/
└── ...
```

## Important: Required Before Running Tests

This **must be completed before running `ctest`** in the build directory. The integration tests expect the data to be available at:

```
<PROJECT_ROOT>/test/integration/data/
```

If the data directory is missing, the integration tests will fail.
