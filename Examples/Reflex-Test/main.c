#include <stdio.h>

#include "Reflex.h"

#define PRINTLN						puts
#define PRINTF						printf

uint32_t assertResult = 0;
#define assert(TYPE, ...)			if((assertResult = Assert_ ##TYPE (__VA_ARGS__, __LINE__)) != Str_Ok) return assertResult

#define ARRAY_LEN(ARR)			    (sizeof(ARR) / sizeof(ARR[0]))

#define setValue(IDX, TYPE, VAL)    values[IDX].Type = Param_ValueType_ ##TYPE;\
                                    values[IDX].TYPE = VAL

typedef uint32_t Test_Result;
typedef Test_Result (*Test_Fn)(void);

Test_Result Assert_Str(const char* str1, const char* str2, uint16_t line);
void Result_print(Test_Result result);


Test_Result Test_Primary_Serielize(void);
Test_Result Test_Serielize(void);

const Test_Fn Tests[] = {
    Test_Primary_Serielize,
    Test_Serielize,
};
const uint32_t Tests_Len = sizeof(Tests) / sizeof(Tests[0]);

int main()
{
    int testIndex;
	int countTestError = 0;
	Test_Result res;

	for (testIndex = 0; testIndex < Tests_Len; testIndex++) {
		PRINTF("---------------- Beginning Test[%d]------------ \r\n", testIndex);
		res = Tests[testIndex]();
		PRINTF("Test Result: %s\r\n", res ? "Error" : "Ok");
		if (res) {
			Result_print(res);
			countTestError++;
		}
		PRINTF("---------------------------------------------- \r\n", testIndex);
	}
	PRINTLN("Test Done\r\n");
	PRINTF("Tests Errors: %d\r\n", countTestError);
}

// ---------------- Test Serialize ---------------
void*       addressMap[12] = {0};
uint8_t     addressMapIndex = 0;

void Reflex_checkAddress(Reflex* reflex, void* out, void* value, Reflex_Type type, Reflex_LenType len, Reflex_LenType len2) {
    if (addressMap[addressMapIndex] != value) {
        PRINTF("Address not match: %X != %X\n", addressMap[addressMapIndex], value);
    }
    addressMapIndex++;
}

const Reflex_SerializeFn SERIALIZE[] = {
    Reflex_checkAddress,
};

typedef struct {
    uint32_t    X;
    float       Y;
    uint8_t     Z;
} PrimaryTemp1;
const uint8_t PrimaryTemp1_FMT[] = {
    Reflex_Type_Primary_UInt32,
    Reflex_Type_Primary_Float,
    Reflex_Type_Primary_UInt8,
    Reflex_Type_Unknown,
};

typedef struct {
    uint16_t    X;
    float       Y;
    uint8_t     Z;
} PrimaryTemp2;
const uint8_t PrimaryTemp2_FMT[] = {
    Reflex_Type_Primary_UInt16,
    Reflex_Type_Primary_Float,
    Reflex_Type_Primary_UInt8,
    Reflex_Type_Unknown,
};

typedef struct {
    uint16_t    A;
    float       B;
    uint8_t     C;
    uint8_t     D;
} PrimaryTemp3;
const uint8_t PrimaryTemp3_FMT[] = {
    Reflex_Type_Primary_UInt16,
    Reflex_Type_Primary_Float,
    Reflex_Type_Primary_UInt8,
    Reflex_Type_Primary_UInt8,
    Reflex_Type_Unknown,
};

typedef struct {
    uint8_t         V0;
    uint32_t        V1;
    uint16_t        V2;
    int16_t         V3;
    int8_t          V4;
    int64_t         V5;
    uint64_t        V6;
    float           V7;
    double          V8;
    uint8_t         V9;
    uint16_t        V10;
    uint32_t        V11;
} PrimaryTemp4;
const uint8_t PrimaryTemp4_FMT[] = {
    Reflex_Type_Primary_UInt8,
    Reflex_Type_Primary_UInt32,
    Reflex_Type_Primary_UInt16,
    Reflex_Type_Primary_Int16,
    Reflex_Type_Primary_Int8,
    Reflex_Type_Primary_Int64,
    Reflex_Type_Primary_UInt64,
    Reflex_Type_Primary_Float,
    Reflex_Type_Primary_Double,
    Reflex_Type_Primary_UInt8,
    Reflex_Type_Primary_UInt16,
    Reflex_Type_Primary_UInt32,
    Reflex_Type_Unknown,
};


Test_Result Test_Primary_Serielize(void) {
    Reflex reflex = {0};

    PrimaryTemp1 temp1;
    PrimaryTemp2 temp2;
    PrimaryTemp3 temp3;
    PrimaryTemp4 temp4;

    reflex.Serialize = SERIALIZE;
    reflex.RepeatFn = 1;

    addressMapIndex = 0;
    addressMap[0] = &temp1.X;
    addressMap[1] = &temp1.Y;
    addressMap[2] = &temp1.Z;
    reflex.PrimaryFmt = PrimaryTemp1_FMT;
    Reflex_scanPrimary(&reflex, &temp1);

    addressMapIndex = 0;
    addressMap[0] = &temp2.X;
    addressMap[1] = &temp2.Y;
    addressMap[2] = &temp2.Z;
    reflex.PrimaryFmt = PrimaryTemp2_FMT;
    Reflex_scanPrimary(&reflex, &temp2);

    addressMapIndex = 0;
    addressMap[0] = &temp3.A;
    addressMap[1] = &temp3.B;
    addressMap[2] = &temp3.C;
    addressMap[3] = &temp3.D;
    reflex.PrimaryFmt = PrimaryTemp3_FMT;
    Reflex_scanPrimary(&reflex, &temp3);

    addressMapIndex = 0;
    addressMap[0] = &temp4.V0;
    addressMap[1] = &temp4.V1;
    addressMap[2] = &temp4.V2;
    addressMap[3] = &temp4.V3;
    addressMap[4] = &temp4.V4;
    addressMap[5] = &temp4.V5;
    addressMap[6] = &temp4.V6;
    addressMap[7] = &temp4.V7;
    addressMap[8] = &temp4.V8;
    addressMap[9] = &temp4.V9;
    addressMap[10] = &temp4.V10;
    addressMap[11] = &temp4.V11;
    reflex.PrimaryFmt = PrimaryTemp4_FMT;
    Reflex_scanPrimary(&reflex, &temp4);

    return 0;
}
// ----------------------------- Test Serielize ---------------------
typedef struct {
    uint8_t*        V0;
    char            V1[10];
    uint32_t        V2;
    char*           V3[4];
    char            V4[4][32];
} Model1;
const Reflex_TypeParams Model1_FMT[] = {
    REFLEX_TYPE_PARAMS(Reflex_Type_Pointer_UInt8, 0, 0),
    REFLEX_TYPE_PARAMS(Reflex_Type_Array_Char, 10, 0),
    REFLEX_TYPE_PARAMS(Reflex_Type_Primary_UInt32, 0, 0),
    REFLEX_TYPE_PARAMS(Reflex_Type_PointerArray_Char, 4, 0),
    REFLEX_TYPE_PARAMS(Reflex_Type_2DArray_Char, 4, 32),
};
const uint8_t Model1_FMT_Len = sizeof(Model1_FMT) / sizeof(Model1_FMT[0]);

Test_Result Test_Serielize(void) {
    Reflex reflex;
    Model1 temp1;

    reflex.Serialize = SERIALIZE;
    reflex.RepeatFn = 1;

    addressMapIndex = 0;
    addressMap[0] = &temp1.V0;
    addressMap[1] = &temp1.V1;
    addressMap[2] = &temp1.V2;
    addressMap[3] = &temp1.V3;
    addressMap[4] = &temp1.V4;
    reflex.Fmt = Model1_FMT;
    reflex.VariablesLength = Model1_FMT_Len;
    Reflex_scan(&reflex, &temp1);

    return 0;
}

void Result_print(Test_Result result) {
    PRINTF("Line: %u, Index: %u\r\n", result >> 16, result & 0xFFFF);
}

