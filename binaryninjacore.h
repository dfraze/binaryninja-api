// Copyright (c) 2015-2016 Vector 35 LLC
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

#ifndef __BINARYNINJACORE_H__
#define __BINARYNINJACORE_H__

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#ifdef __GNUC__
#  ifdef BINARYNINJACORE_LIBRARY
#    define BINARYNINJACOREAPI __attribute__((visibility("default")))
#  else
#    define BINARYNINJACOREAPI
#  endif
#  define BINARYNINJAPLUGIN __attribute__((visibility("default")))
#else
#ifdef _MSC_VER
#  ifdef BINARYNINJACORE_LIBRARY
#    define BINARYNINJACOREAPI __declspec(dllexport)
#  else
#    define BINARYNINJACOREAPI
#  endif
#  define BINARYNINJAPLUGIN __declspec(dllexport)
#else
#define BINARYNINJACOREAPI
#endif
#endif

#define BN_MAX_INSTRUCTION_LENGTH   256
#define BN_DEFAULT_NSTRUCTION_LENGTH 16
#define BN_DEFAULT_OPCODE_DISPLAY   8
#define BN_MAX_INSTRUCTION_BRANCHES 3

#define BN_MAX_STORED_DATA_LENGTH   0x3fffffff
#define BN_NULL_ID                  -1

#define LLIL_TEMP(n)                (0x80000000 | (uint32_t)(n))
#define LLIL_REG_IS_TEMP(n)         (((n) & 0x80000000) != 0)
#define LLIL_GET_TEMP_REG_INDEX(n)  ((n) & 0x7fffffff)
#define BN_INVALID_REGISTER         0xffffffff

#define BN_INVALID_OPERAND          0xffffffff

#define BN_DEFAULT_MIN_STRING_LENGTH 4
#define BN_MAX_STRING_LENGTH         128

#ifdef __cplusplus
extern "C"
{
#endif
	enum BNPluginLoadOrder
	{
		EarlyPluginLoadOrder,
		NormalPluginLoadOrder,
		LatePluginLoadOrder
	};

	typedef bool (*BNCorePluginInitFunction)(void);
	typedef void (*BNCorePluginDependencyFunction)(void);

	struct BNDataBuffer;
	struct BNBinaryView;
	struct BNBinaryViewType;
	struct BNBinaryReader;
	struct BNBinaryWriter;
	struct BNFileMetadata;
	struct BNTransform;
	struct BNArchitecture;
	struct BNFunction;
	struct BNBasicBlock;
	struct BNFunctionGraph;
	struct BNFunctionGraphBlock;
	struct BNSymbol;
	struct BNTemporaryFile;
	struct BNLowLevelILFunction;
	struct BNType;
	struct BNStructure;
	struct BNEnumeration;
	struct BNCallingConvention;
	struct BNPlatform;
	struct BNAnalysisCompletionEvent;
	struct BNDisassemblySettings;
	struct BNScriptingProvider;
	struct BNScriptingInstance;
	struct BNMainThreadAction;

	//! Console log levels
	enum BNLogLevel
	{
		DebugLog = 0,   //! Debug logging level, most verbose logging level
		InfoLog = 1,    //! Information logging level, default logging level
		WarningLog = 2, //! Warning logging level, messages show with warning icon in the UI
		ErrorLog = 3,   //! Error logging level, messages show with error icon in the UI
		AlertLog = 4    //! Alert logging level, messages are displayed with popup message box in the UI
	};

	enum BNEndianness
	{
		LittleEndian = 0,
		BigEndian = 1
	};

	enum BNModificationStatus
	{
		Original = 0,
		Changed = 1,
		Inserted = 2
	};

	enum BNTransformType
	{
		BinaryCodecTransform = 0, // Two-way transform of data, binary input/output
		TextCodecTransform = 1, // Two-way transform of data, encoder output is text
		UnicodeCodecTransform = 2, // Two-way transform of data, encoder output is Unicode string (as UTF8)
		DecodeTransform = 3, // One-way decode only
		BinaryEncodeTransform = 4, // One-way encode only, output is binary
		TextEncodeTransform = 5, // One-way encode only, output is text
		EncryptTransform = 6, // Two-way encryption
		InvertingTransform = 7, // Transform that can be undone by performing twice
		HashTransform = 8 // Hash function
	};

	enum BNBranchType
	{
		UnconditionalBranch = 0,
		FalseBranch = 1,
		TrueBranch = 2,
		CallDestination = 3,
		FunctionReturn = 4,
		SystemCall = 5,
		IndirectBranch = 6,
		UnresolvedBranch = 127
	};

	enum BNInstructionTextTokenType
	{
		TextToken = 0,
		InstructionToken = 1,
		OperandSeparatorToken = 2,
		RegisterToken = 3,
		IntegerToken = 4,
		PossibleAddressToken = 5,
		BeginMemoryOperandToken = 6,
		EndMemoryOperandToken = 7,
		FloatingPointToken = 8,
		AnnotationToken = 9,
		CodeRelativeAddressToken = 10,
		StackVariableTypeToken = 11,
		DataVariableTypeToken = 12,
		FunctionReturnTypeToken = 13,
		FunctionAttributeToken = 14,
		ArgumentTypeToken = 15,
		ArgumentNameToken = 16,
		HexDumpByteValueToken = 17,
		HexDumpSkippedByteToken = 18,
		HexDumpInvalidByteToken = 19,
		HexDumpTextToken = 20,
		OpcodeToken = 21,
		StringToken = 22,
		CharacterConstantToken = 23,

		// The following are output by the analysis system automatically, these should
		// not be used directly by the architecture plugins
		CodeSymbolToken = 64,
		DataSymbolToken = 65,
		StackVariableToken = 66,
		ImportToken = 67,
		AddressDisplayToken = 68
	};

	enum BNLinearDisassemblyLineType
	{
		BlankLineType,
		CodeDisassemblyLineType,
		DataVariableLineType,
		HexDumpLineType,
		FunctionHeaderLineType,
		FunctionHeaderStartLineType,
		FunctionHeaderEndLineType,
		FunctionContinuationLineType,
		StackVariableLineType,
		StackVariableListEndLineType,
		FunctionEndLineType
	};

	enum BNSymbolType
	{
		FunctionSymbol = 0,
		ImportAddressSymbol = 1,
		ImportedFunctionSymbol = 2,
		DataSymbol = 3,
		ImportedDataSymbol = 4
	};

	enum BNActionType
	{
		TemporaryAction = 0,
		DataModificationAction = 1,
		AnalysisAction = 2,
		DataModificationAndAnalysisAction = 3
	};

	enum BNLowLevelILOperation
	{
		LLIL_NOP,
		LLIL_SET_REG,
		LLIL_SET_REG_SPLIT,
		LLIL_SET_FLAG,
		LLIL_LOAD,
		LLIL_STORE,
		LLIL_PUSH,
		LLIL_POP,
		LLIL_REG,
		LLIL_CONST,
		LLIL_FLAG,
		LLIL_FLAG_BIT,
		LLIL_ADD,
		LLIL_ADC,
		LLIL_SUB,
		LLIL_SBB,
		LLIL_AND,
		LLIL_OR,
		LLIL_XOR,
		LLIL_LSL,
		LLIL_LSR,
		LLIL_ASR,
		LLIL_ROL,
		LLIL_RLC,
		LLIL_ROR,
		LLIL_RRC,
		LLIL_MUL,
		LLIL_MULU_DP,
		LLIL_MULS_DP,
		LLIL_DIVU,
		LLIL_DIVU_DP,
		LLIL_DIVS,
		LLIL_DIVS_DP,
		LLIL_MODU,
		LLIL_MODU_DP,
		LLIL_MODS,
		LLIL_MODS_DP,
		LLIL_NEG,
		LLIL_NOT,
		LLIL_SX,
		LLIL_ZX,
		LLIL_JUMP,
		LLIL_JUMP_TO,
		LLIL_CALL,
		LLIL_RET,
		LLIL_NORET,
		LLIL_IF,
		LLIL_GOTO,
		LLIL_FLAG_COND,
		LLIL_CMP_E,
		LLIL_CMP_NE,
		LLIL_CMP_SLT,
		LLIL_CMP_ULT,
		LLIL_CMP_SLE,
		LLIL_CMP_ULE,
		LLIL_CMP_SGE,
		LLIL_CMP_UGE,
		LLIL_CMP_SGT,
		LLIL_CMP_UGT,
		LLIL_TEST_BIT,
		LLIL_BOOL_TO_INT,
		LLIL_SYSCALL,
		LLIL_BP,
		LLIL_TRAP,
		LLIL_UNDEF,
		LLIL_UNIMPL,
		LLIL_UNIMPL_MEM
	};

	enum BNLowLevelILFlagCondition
	{
		LLFC_E,
		LLFC_NE,
		LLFC_SLT,
		LLFC_ULT,
		LLFC_SLE,
		LLFC_ULE,
		LLFC_SGE,
		LLFC_UGE,
		LLFC_SGT,
		LLFC_UGT,
		LLFC_NEG,
		LLFC_POS,
		LLFC_O,
		LLFC_NO
	};

	enum BNFlagRole
	{
		SpecialFlagRole = 0,
		ZeroFlagRole = 1,
		PositiveSignFlagRole = 2,
		NegativeSignFlagRole = 3,
		CarryFlagRole = 4,
		OverflowFlagRole = 5,
		HalfCarryFlagRole = 6,
		EvenParityFlagRole = 7,
		OddParityFlagRole = 8
	};

	enum BNFunctionGraphType
	{
		NormalFunctionGraph = 0,
		LowLevelILFunctionGraph = 1,
		LiftedILFunctionGraph = 2
	};

	enum BNDisassemblyOption
	{
		ShowAddress = 0,
		ShowOpcode = 1,
		ExpandLongOpcode = 2,

		// Linear disassembly options
		GroupLinearDisassemblyFunctions = 64,

		// Debugging options
		ShowBasicBlockRegisterState = 128,
		ShowFlagUsage = 129
	};

	enum BNTypeClass
	{
		VoidTypeClass = 0,
		BoolTypeClass = 1,
		IntegerTypeClass = 2,
		FloatTypeClass = 3,
		StructureTypeClass = 4,
		EnumerationTypeClass = 5,
		PointerTypeClass = 6,
		ArrayTypeClass = 7,
		FunctionTypeClass = 8,
		VarArgsTypeClass = 9,
		ValueTypeClass = 10
	};

	enum BNStructureType
	{
		ClassStructureType = 0,
		StructStructureType = 1,
		UnionStructureType = 2
	};

	enum BNMemberScope {
		NoScope,
		StaticScope,
		VirtualScope,
		ThunkScope
	};

	enum BNMemberAccess
	{
		NoAccess,
		PrivateAccess,
		ProtectedAccess,
		PublicAccess
	};

	enum BNReferenceType
	{
		PointerReferenceType = 0,
		ReferenceReferenceType = 1,
		RValueReferenceType = 2
	};

	enum BNPointerSuffix
	{
		Ptr64Suffix,
		UnalignedSuffix,
		RestrictSuffix,
		ReferenceSuffix,
		LvalueSuffix
	};

	enum BNNameType
	{
		NoNameType,
		ConstructorNameType,
		DestructorNameType,
		OperatorNewNameType,
		OperatorDeleteNameType,
		OperatorAssignNameType,
		OperatorRightShiftNameType,
		OperatorLeftShiftNameType,
		OperatorNotNameType,
		OperatorEqualNameType,
		OperatorNotEqualNameType,
		OperatorArrayNameType,
		OperatorArrowNameType,
		OperatorStarNameType,
		OperatorIncrementNameType,
		OperatorDecrementNameType,
		OperatorMinusNameType,
		OperatorPlusNameType,
		OperatorBitAndNameType,
		OperatorArrowStarNameType,
		OperatorDivideNameType,
		OperatorModulusNameType,
		OperatorLessThanNameType,
		OperatorLessThanEqualNameType,
		OperatorGreaterThanNameType,
		OperatorGreaterThanEqualNameType,
		OperatorCommaNameType,
		OperatorParenthesesNameType,
		OperatorTildeNameType,
		OperatorXorNameType,
		OperatorBitOrNameType,
		OperatorLogicalAndNameType,
		OperatorLogicalOrNameType,
		OperatorStarEqualNameType,
		OperatorPlusEqualNameType,
		OperatorMinusEqualNameType,
		OperatorDivideEqualNameType,
		OperatorModulusEqualNameType,
		OperatorRightShiftEqualNameType,
		OperatorLeftShiftEqualNameType,
		OperatorAndEqualNameType,
		OperatorOrEqualNameType,
		OperatorXorEqualNameType,
		VFTableNameType,
		VBTableNameType,
		VCallNameType,
		TypeofNameType,
		LocalStaticGuardNameType,
		StringNameType,
		VBaseDestructorNameType,
		VectorDeletingDestructorNameType,
		DefaultConstructorClosureNameType,
		ScalarDeletingDestructorNameType,
		VectorConstructorIteratorNameType,
		VectorDestructorIteratorNameType,
		VectorVBaseConstructorIteratoreNameType,
		VirtualDisplacementMapNameType,
		EHVectorConstructorIteratorNameType,
		EHVectorDestructorIteratorNameType,
		EHVectorVBaseConstructorIteratorNameType,
		CopyConstructorClosureNameType,
		UDTReturningNameType,
		LocalVFTableNameType,
		LocalVFTableConstructorClosureNameType,
		OperatorNewArrayNameType,
		OperatorDeleteArrayNameType,
		PlacementDeleteClosureNameType,
		PlacementDeleteClosureArrayNameType,
		OperatorReturnTypeNameType,
		RttiTypeDescriptor,
		RttiBaseClassDescriptor,
		RttiBaseClassArray,
		RttiClassHeirarchyDescriptor,
		RttiCompleteObjectLocator
	};

	enum BNCallingConventionName
	{
		NoCallingConvention,
		CdeclCallingConvention,
		PascalCallingConvention,
		ThisCallCallingConvention,
		STDCallCallingConvention,
		FastcallCallingConvention,
		CLRCallCallingConvention,
		EabiCallCallingConvention,
		VectorCallCallingConvention
	};

	enum BNStringType
	{
		AsciiString = 0,
		Utf16String = 1,
		Utf32String = 2
	};

	enum BNIntegerDisplayType
	{
		DefaultIntegerDisplayType,
		BinaryDisplayType,
		SignedOctalDisplayType,
		UnsignedOctalDisplayType,
		SignedDecimalDisplayType,
		UnsignedDecimalDisplayType,
		SignedHexadecimalDisplayType,
		UnsignedHexadecimalDisplayType,
		CharacterConstantDisplayType
	};

	struct BNLowLevelILInstruction
	{
		BNLowLevelILOperation operation;
		size_t size;
		uint32_t flags;
		uint32_t sourceOperand;
		uint64_t operands[4];
		uint64_t address;
	};

	struct BNLowLevelILLabel
	{
		bool resolved;
		size_t ref;
		size_t operand;
	};

	enum BNImplicitRegisterExtend
	{
		NoExtend,
		ZeroExtendToFullWidth,
		SignExtendToFullWidth
	};

	struct BNRegisterInfo
	{
		uint32_t fullWidthRegister;
		size_t offset;
		size_t size;
		BNImplicitRegisterExtend extend;
	};

	enum BNRegisterValueType
	{
		EntryValue,
		OffsetFromEntryValue,
		ConstantValue,
		StackFrameOffset,
		UndeterminedValue,
		OffsetFromUndeterminedValue,
		SignedRangeValue,
		UnsignedRangeValue,
		LookupTableValue,
		ComparisonResultValue
	};

	struct BNLookupTableEntry
	{
		int64_t* fromValues;
		size_t fromCount;
		int64_t toValue;
	};

	struct BNRegisterValue
	{
		BNRegisterValueType state;
		uint32_t reg; // For EntryValue and OffsetFromEntryValue, the original input register
		int64_t value; // Offset for OffsetFromEntryValue, StackFrameOffset or RangeValue, value of register for ConstantValue
		uint64_t rangeStart, rangeEnd, rangeStep; // Range of register, inclusive
		BNLookupTableEntry* table; // Number of entries in rangeEnd
	};

	struct BNRegisterOrConstant
	{
		bool constant;
		uint32_t reg;
		uint64_t value;
	};

	struct BNDataVariable
	{
		uint64_t address;
		BNType* type;
		bool autoDiscovered;
	};

	// Callbacks
	struct BNLogListener
	{
		void* context;
		void (*log)(void* ctxt, BNLogLevel level, const char* msg);
		void (*close)(void* ctxt);
		BNLogLevel (*getLogLevel)(void* ctxt);
	};

	struct BNNavigationHandler
	{
		void* context;
		char* (*getCurrentView)(void* ctxt);
		uint64_t (*getCurrentOffset)(void* ctxt);
		bool (*navigate)(void* ctxt, const char* view, uint64_t offset);
	};

	struct BNBinaryDataNotification
	{
		void* context;
		void (*dataWritten)(void* ctxt, BNBinaryView* view, uint64_t offset, size_t len);
		void (*dataInserted)(void* ctxt, BNBinaryView* view, uint64_t offset, size_t len);
		void (*dataRemoved)(void* ctxt, BNBinaryView* view, uint64_t offset, uint64_t len);
		void (*functionAdded)(void* ctxt, BNBinaryView* view, BNFunction* func);
		void (*functionRemoved)(void* ctxt, BNBinaryView* view, BNFunction* func);
		void (*functionUpdated)(void* ctxt, BNBinaryView* view, BNFunction* func);
		void (*dataVariableAdded)(void* ctxt, BNBinaryView* view, BNDataVariable* var);
		void (*dataVariableRemoved)(void* ctxt, BNBinaryView* view, BNDataVariable* var);
		void (*dataVariableUpdated)(void* ctxt, BNBinaryView* view, BNDataVariable* var);
		void (*stringFound)(void* ctxt, BNBinaryView* view, BNStringType type, uint64_t offset, size_t len);
		void (*stringRemoved)(void* ctxt, BNBinaryView* view, BNStringType type, uint64_t offset, size_t len);
	};

	struct BNFileAccessor
	{
		void* context;
		uint64_t (*getLength)(void* ctxt);
		size_t (*read)(void* ctxt, void* dest, uint64_t offset, size_t len);
		size_t (*write)(void* ctxt, uint64_t offset, const void* src, size_t len);
	};

	struct BNCustomBinaryView
	{
		void* context;
		bool (*init)(void* ctxt);
		void (*freeObject)(void* ctxt);
		size_t (*read)(void* ctxt, void* dest, uint64_t offset, size_t len);
		size_t (*write)(void* ctxt, uint64_t offset, const void* src, size_t len);
		size_t (*insert)(void* ctxt, uint64_t offset, const void* src, size_t len);
		size_t (*remove)(void* ctxt, uint64_t offset, uint64_t len);
		BNModificationStatus (*getModification)(void* ctxt, uint64_t offset);
		bool (*isValidOffset)(void* ctxt, uint64_t offset);
		bool (*isOffsetReadable)(void* ctxt, uint64_t offset);
		bool (*isOffsetWritable)(void* ctxt, uint64_t offset);
		bool (*isOffsetExecutable)(void* ctxt, uint64_t offset);
		uint64_t (*getNextValidOffset)(void* ctxt, uint64_t offset);
		uint64_t (*getStart)(void* ctxt);
		uint64_t (*getLength)(void* ctxt);
		uint64_t (*getEntryPoint)(void* ctxt);
		bool (*isExecutable)(void* ctxt);
		BNEndianness (*getDefaultEndianness)(void* ctxt);
		size_t (*getAddressSize)(void* ctxt);
		bool (*save)(void* ctxt, BNFileAccessor* accessor);
	};

	struct BNCustomBinaryViewType
	{
		void* context;
		BNBinaryView* (*create)(void* ctxt, BNBinaryView* data);
		bool (*isValidForData)(void* ctxt, BNBinaryView* data);
	};

	struct BNTransformParameterInfo
	{
		char* name;
		char* longName;
		size_t fixedLength; // Variable length if zero
	};

	struct BNTransformParameter
	{
		const char* name;
		BNDataBuffer* value;
	};

	struct BNCustomTransform
	{
		void* context;
		BNTransformParameterInfo* (*getParameters)(void* ctxt, size_t* count);
		void (*freeParameters)(BNTransformParameterInfo* params, size_t count);
		bool (*decode)(void* ctxt, BNDataBuffer* input, BNDataBuffer* output, BNTransformParameter* params, size_t paramCount);
		bool (*encode)(void* ctxt, BNDataBuffer* input, BNDataBuffer* output, BNTransformParameter* params, size_t paramCount);
	};

	struct BNInstructionInfo
	{
		size_t length;
		size_t branchCount;
		bool branchDelay;
		BNBranchType branchType[BN_MAX_INSTRUCTION_BRANCHES];
		uint64_t branchTarget[BN_MAX_INSTRUCTION_BRANCHES];
		BNArchitecture* branchArch[BN_MAX_INSTRUCTION_BRANCHES]; // If null, same architecture as instruction
	};

	struct BNInstructionTextToken
	{
		BNInstructionTextTokenType type;
		char* text;
		uint64_t value;
		size_t size, operand;
	};

	struct BNInstructionTextLine
	{
		BNInstructionTextToken* tokens;
		size_t count;
	};

	struct BNCustomArchitecture
	{
		void* context;
		void (*init)(void* context, BNArchitecture* obj);
		BNEndianness (*getEndianness)(void* ctxt);
		size_t (*getAddressSize)(void* ctxt);
		size_t (*getDefaultIntegerSize)(void* ctxt);
		size_t (*getMaxInstructionLength)(void* ctxt);
		size_t (*getOpcodeDisplayLength)(void* ctxt);
		bool (*getInstructionInfo)(void* ctxt, const uint8_t* data, uint64_t addr, size_t maxLen, BNInstructionInfo* result);
		bool (*getInstructionText)(void* ctxt, const uint8_t* data, uint64_t addr, size_t* len,
		                           BNInstructionTextToken** result, size_t* count);
		void (*freeInstructionText)(BNInstructionTextToken* tokens, size_t count);
		bool (*getInstructionLowLevelIL)(void* ctxt, const uint8_t* data, uint64_t addr, size_t* len, BNLowLevelILFunction* il);
		char* (*getRegisterName)(void* ctxt, uint32_t reg);
		char* (*getFlagName)(void* ctxt, uint32_t flag);
		char* (*getFlagWriteTypeName)(void* ctxt, uint32_t flags);
		uint32_t* (*getFullWidthRegisters)(void* ctxt, size_t* count);
		uint32_t* (*getAllRegisters)(void* ctxt, size_t* count);
		uint32_t* (*getAllFlags)(void* ctxt, size_t* count);
		uint32_t* (*getAllFlagWriteTypes)(void* ctxt, size_t* count);
		BNFlagRole (*getFlagRole)(void* ctxt, uint32_t flag);
		uint32_t* (*getFlagsRequiredForFlagCondition)(void* ctxt, BNLowLevelILFlagCondition cond, size_t* count);
		uint32_t* (*getFlagsWrittenByFlagWriteType)(void* ctxt, uint32_t writeType, size_t* count);
		size_t (*getFlagWriteLowLevelIL)(void* ctxt, BNLowLevelILOperation op, size_t size, uint32_t flagWriteType,
			uint32_t flag, BNRegisterOrConstant* operands, size_t operandCount, BNLowLevelILFunction* il);
		size_t (*getFlagConditionLowLevelIL)(void* ctxt, BNLowLevelILFlagCondition cond, BNLowLevelILFunction* il);
		void (*freeRegisterList)(void* ctxt, uint32_t* regs);
		void (*getRegisterInfo)(void* ctxt, uint32_t reg, BNRegisterInfo* result);
		uint32_t (*getStackPointerRegister)(void* ctxt);
		uint32_t (*getLinkRegister)(void* ctxt);

		bool (*assemble)(void* ctxt, const char* code, uint64_t addr, BNDataBuffer* result, char** errors);

		bool (*isNeverBranchPatchAvailable)(void* ctxt, const uint8_t* data, uint64_t addr, size_t len);
		bool (*isAlwaysBranchPatchAvailable)(void* ctxt, const uint8_t* data, uint64_t addr, size_t len);
		bool (*isInvertBranchPatchAvailable)(void* ctxt, const uint8_t* data, uint64_t addr, size_t len);
		bool (*isSkipAndReturnZeroPatchAvailable)(void* ctxt, const uint8_t* data, uint64_t addr, size_t len);
		bool (*isSkipAndReturnValuePatchAvailable)(void* ctxt, const uint8_t* data, uint64_t addr, size_t len);

		bool (*convertToNop)(void* ctxt, uint8_t* data, uint64_t addr, size_t len);
		bool (*alwaysBranch)(void* ctxt, uint8_t* data, uint64_t addr, size_t len);
		bool (*invertBranch)(void* ctxt, uint8_t* data, uint64_t addr, size_t len);
		bool (*skipAndReturnValue)(void* ctxt, uint8_t* data, uint64_t addr, size_t len, uint64_t value);
	};

	struct BNBasicBlockEdge
	{
		BNBranchType type;
		uint64_t target;
		BNArchitecture* arch;
	};

	struct BNPoint
	{
		float x;
		float y;
	};

	struct BNFunctionGraphEdge
	{
		BNBranchType type;
		uint64_t target;
		BNArchitecture* arch;
		BNPoint* points;
		size_t pointCount;
	};

	struct BNDisassemblyTextLine
	{
		uint64_t addr;
		BNInstructionTextToken* tokens;
		size_t count;
	};

	struct BNLinearDisassemblyLine
	{
		BNLinearDisassemblyLineType type;
		BNFunction* function;
		BNBasicBlock* block;
		size_t lineOffset;
		BNDisassemblyTextLine contents;
	};

	struct BNLinearDisassemblyPosition
	{
		BNFunction* function;
		BNBasicBlock* block;
		uint64_t address;
	};

	struct BNReferenceSource
	{
		BNFunction* func;
		BNArchitecture* arch;
		uint64_t addr;
	};

	struct BNUndoAction
	{
		BNActionType type;
		void* context;
		void (*freeObject)(void* ctxt);
		void (*undo)(void* ctxt, BNBinaryView* data);
		void (*redo)(void* ctxt, BNBinaryView* data);
		char* (*serialize)(void* ctxt);
	};

	struct BNNameAndType
	{
		char* name;
		BNType* type;
	};

	struct BNStructureMember
	{
		BNType* type;
		char* name;
		uint64_t offset;
	};

	struct BNEnumerationMember
	{
		char* name;
		uint64_t value;
		bool isDefault;
	};

	struct BNFunctionRecognizer
	{
		void* context;
		bool (*recognizeLowLevelIL)(void* ctxt, BNBinaryView* data, BNFunction* func, BNLowLevelILFunction* il);
	};

	struct BNTypeParserResult
	{
		BNNameAndType* types;
		BNNameAndType* variables;
		BNNameAndType* functions;
		size_t typeCount, variableCount, functionCount;
	};

	enum BNUpdateResult
	{
		UpdateFailed = 0,
		UpdateSuccess = 1,
		AlreadyUpToDate = 2
	};

	struct BNUpdateChannel
	{
		char* name;
		char* description;
		char* latestVersion;
	};

	struct BNUpdateVersion
	{
		char* version;
		char* notes;
		uint64_t time;
	};

	struct BNStringReference
	{
		BNStringType type;
		uint64_t start;
		size_t length;
	};

	enum BNPluginCommandType
	{
		DefaultPluginCommand,
		AddressPluginCommand,
		RangePluginCommand,
		FunctionPluginCommand
	};

	struct BNPluginCommand
	{
		char* name;
		char* description;
		BNPluginCommandType type;
		void* context;

		void (*defaultCommand)(void* ctxt, BNBinaryView* view);
		void (*addressCommand)(void* ctxt, BNBinaryView* view, uint64_t addr);
		void (*rangeCommand)(void* ctxt, BNBinaryView* view, uint64_t addr, uint64_t len);
		void (*functionCommand)(void* ctxt, BNBinaryView* view, BNFunction* func);

		bool (*defaultIsValid)(void* ctxt, BNBinaryView* view);
		bool (*addressIsValid)(void* ctxt, BNBinaryView* view, uint64_t addr);
		bool (*rangeIsValid)(void* ctxt, BNBinaryView* view, uint64_t addr, uint64_t len);
		bool (*functionIsValid)(void* ctxt, BNBinaryView* view, BNFunction* func);
	};

	struct BNCustomCallingConvention
	{
		void* context;

		void (*freeObject)(void* ctxt);

		uint32_t* (*getCallerSavedRegisters)(void* ctxt, size_t* count);
		uint32_t* (*getIntegerArgumentRegisters)(void* ctxt, size_t* count);
		uint32_t* (*getFloatArgumentRegisters)(void* ctxt, size_t* count);
		void (*freeRegisterList)(void* ctxt, uint32_t* regs);

		bool (*areArgumentRegistersSharedIndex)(void* ctxt);
		bool (*isStackReservedForArgumentRegisters)(void* ctxt);

		uint32_t (*getIntegerReturnValueRegister)(void* ctxt);
		uint32_t (*getHighIntegerReturnValueRegister)(void* ctxt);
		uint32_t (*getFloatReturnValueRegister)(void* ctxt);
	};

	struct BNStackVariable
	{
		BNType* type;
		char* name;
		int64_t offset;
		bool autoDefined;
	};

	struct BNStackVariableReference
	{
		uint32_t sourceOperand;
		BNType* type;
		char* name;
		int64_t startingOffset;
		int64_t referencedOffset;
	};

	struct BNIndirectBranchInfo
	{
		BNArchitecture* sourceArch;
		uint64_t sourceAddr;
		BNArchitecture* destArch;
		uint64_t destAddr;
		bool autoDefined;
	};

	struct BNArchitectureAndAddress
	{
		BNArchitecture* arch;
		uint64_t address;
	};

	enum BNAnalysisState
	{
		IdleState,
		DisassembleState,
		AnalyzeState
	};

	struct BNAnalysisProgress
	{
		BNAnalysisState state;
		size_t count, total;
	};

	enum BNFindFlag
	{
		NoFindFlags = 0,
		FindCaseInsensitive = 1
	};

	enum BNScriptingProviderInputReadyState
	{
		NotReadyForInput,
		ReadyForScriptExecution,
		ReadyForScriptProgramInput
	};

	enum BNScriptingProviderExecuteResult
	{
		InvalidScriptInput,
		IncompleteScriptInput,
		SuccessfulScriptExecution
	};

	struct BNScriptingInstanceCallbacks
	{
		void* context;
		void (*destroyInstance)(void* ctxt);
		BNScriptingProviderExecuteResult (*executeScriptInput)(void* ctxt, const char* input);
		void (*setCurrentBinaryView)(void* ctxt, BNBinaryView* view);
		void (*setCurrentFunction)(void* ctxt, BNFunction* func);
		void (*setCurrentBasicBlock)(void* ctxt, BNBasicBlock* block);
		void (*setCurrentAddress)(void* ctxt, uint64_t addr);
		void (*setCurrentSelection)(void* ctxt, uint64_t begin, uint64_t end);
	};

	struct BNScriptingProviderCallbacks
	{
		void* context;
		BNScriptingInstance* (*createInstance)(void* ctxt);

	};

	struct BNScriptingOutputListener
	{
		void* context;
		void (*output)(void* ctxt, const char* text);
		void (*error)(void* ctxt, const char* text);
		void (*inputReadyStateChanged)(void* ctxt, BNScriptingProviderInputReadyState state);
	};

	struct BNMainThreadCallbacks
	{
		void* context;
		void (*addAction)(void* ctxt, BNMainThreadAction* action);
	};

	BINARYNINJACOREAPI char* BNAllocString(const char* contents);
	BINARYNINJACOREAPI void BNFreeString(char* str);

	BINARYNINJACOREAPI void BNShutdown(void);

	BINARYNINJACOREAPI char* BNGetVersionString(void);
	BINARYNINJACOREAPI uint32_t BNGetBuildId(void);

	BINARYNINJACOREAPI bool BNIsLicenseValidated(void);

	// Plugin initialization
	BINARYNINJACOREAPI void BNInitCorePlugins(void);
	BINARYNINJACOREAPI void BNInitUserPlugins(void);
	BINARYNINJACOREAPI char* BNGetBundledPluginDirectory(void);
	BINARYNINJACOREAPI void BNSetBundledPluginDirectory(const char* path);
	BINARYNINJACOREAPI char* BNGetUserPluginDirectory(void);

	BINARYNINJACOREAPI char* BNGetPathRelativeToBundledPluginDirectory(const char* path);
	BINARYNINJACOREAPI char* BNGetPathRelativeToUserPluginDirectory(const char* path);

	BINARYNINJACOREAPI bool BNExecuteWorkerProcess(const char* path, const char* args[],
	                                               BNDataBuffer* input, char** output, char** error);

	BINARYNINJACOREAPI void BNSetCurrentPluginLoadOrder(BNPluginLoadOrder order);
	BINARYNINJACOREAPI void BNAddRequiredPluginDependency(const char* name);
	BINARYNINJACOREAPI void BNAddOptionalPluginDependency(const char* name);

	// Logging
	BINARYNINJACOREAPI void BNLog(BNLogLevel level, const char* fmt, ...);
	BINARYNINJACOREAPI void BNLogDebug(const char* fmt, ...);
	BINARYNINJACOREAPI void BNLogInfo(const char* fmt, ...);
	BINARYNINJACOREAPI void BNLogWarn(const char* fmt, ...);
	BINARYNINJACOREAPI void BNLogError(const char* fmt, ...);
	BINARYNINJACOREAPI void BNLogAlert(const char* fmt, ...);

	BINARYNINJACOREAPI void BNRegisterLogListener(BNLogListener* listener);
	BINARYNINJACOREAPI void BNUnregisterLogListener(BNLogListener* listener);
	BINARYNINJACOREAPI void BNUpdateLogListeners(void);

	BINARYNINJACOREAPI void BNLogToStdout(BNLogLevel minimumLevel);
	BINARYNINJACOREAPI void BNLogToStderr(BNLogLevel minimumLevel);
	BINARYNINJACOREAPI bool BNLogToFile(BNLogLevel minimumLevel, const char* path, bool append);
	BINARYNINJACOREAPI void BNCloseLogs(void);

	// Temporary files
	BINARYNINJACOREAPI BNTemporaryFile* BNCreateTemporaryFile(void);
	BINARYNINJACOREAPI BNTemporaryFile* BNCreateTemporaryFileWithContents(BNDataBuffer* data);
	BINARYNINJACOREAPI BNTemporaryFile* BNNewTemporaryFileReference(BNTemporaryFile* file);
	BINARYNINJACOREAPI void BNFreeTemporaryFile(BNTemporaryFile* file);
	BINARYNINJACOREAPI char* BNGetTemporaryFilePath(BNTemporaryFile* file);
	BINARYNINJACOREAPI BNDataBuffer* BNGetTemporaryFileContents(BNTemporaryFile* file);

	// Data buffer management
	BINARYNINJACOREAPI BNDataBuffer* BNCreateDataBuffer(const void* data, size_t len);
	BINARYNINJACOREAPI BNDataBuffer* BNDuplicateDataBuffer(BNDataBuffer* buf);
	BINARYNINJACOREAPI void BNFreeDataBuffer(BNDataBuffer* buf);
	BINARYNINJACOREAPI void* BNGetDataBufferContents(BNDataBuffer* buf);
	BINARYNINJACOREAPI void* BNGetDataBufferContentsAt(BNDataBuffer* buf, size_t offset);
	BINARYNINJACOREAPI size_t BNGetDataBufferLength(BNDataBuffer* buf);
	BINARYNINJACOREAPI BNDataBuffer* BNGetDataBufferSlice(BNDataBuffer* buf, size_t start, size_t len);

	BINARYNINJACOREAPI void BNSetDataBufferLength(BNDataBuffer* buf, size_t len);
	BINARYNINJACOREAPI void BNClearDataBuffer(BNDataBuffer* buf);
	BINARYNINJACOREAPI void BNSetDataBufferContents(BNDataBuffer* buf, void* data, size_t len);
	BINARYNINJACOREAPI void BNAssignDataBuffer(BNDataBuffer* dest, BNDataBuffer* src);
	BINARYNINJACOREAPI void BNAppendDataBuffer(BNDataBuffer* dest, BNDataBuffer* src);
	BINARYNINJACOREAPI void BNAppendDataBufferContents(BNDataBuffer* dest, const void* src, size_t len);

	BINARYNINJACOREAPI uint8_t BNGetDataBufferByte(BNDataBuffer* buf, size_t offset);
	BINARYNINJACOREAPI void BNSetDataBufferByte(BNDataBuffer* buf, size_t offset, uint8_t val);

	BINARYNINJACOREAPI char* BNDataBufferToEscapedString(BNDataBuffer* buf);
	BINARYNINJACOREAPI BNDataBuffer* BNDecodeEscapedString(const char* str);
	BINARYNINJACOREAPI char* BNDataBufferToBase64(BNDataBuffer* buf);
	BINARYNINJACOREAPI BNDataBuffer* BNDecodeBase64(const char* str);

	BINARYNINJACOREAPI BNDataBuffer* BNZlibCompress(BNDataBuffer* buf);
	BINARYNINJACOREAPI BNDataBuffer* BNZlibDecompress(BNDataBuffer* buf);

	// File metadata object
	BINARYNINJACOREAPI BNFileMetadata* BNCreateFileMetadata(void);
	BINARYNINJACOREAPI BNFileMetadata* BNNewFileReference(BNFileMetadata* file);
	BINARYNINJACOREAPI void BNFreeFileMetadata(BNFileMetadata* file);
	BINARYNINJACOREAPI void BNCloseFile(BNFileMetadata* file);
	BINARYNINJACOREAPI void BNSetFileMetadataNavigationHandler(BNFileMetadata* file, BNNavigationHandler* handler);
	BINARYNINJACOREAPI bool BNIsFileModified(BNFileMetadata* file);
	BINARYNINJACOREAPI bool BNIsAnalysisChanged(BNFileMetadata* file);
	BINARYNINJACOREAPI void BNMarkFileModified(BNFileMetadata* file);
	BINARYNINJACOREAPI void BNMarkFileSaved(BNFileMetadata* file);

	BINARYNINJACOREAPI bool BNIsBackedByDatabase(BNFileMetadata* file);
	BINARYNINJACOREAPI bool BNCreateDatabase(BNBinaryView* data, const char* path);
	BINARYNINJACOREAPI BNBinaryView* BNOpenExistingDatabase(BNFileMetadata* file, const char* path);
	BINARYNINJACOREAPI bool BNSaveAutoSnapshot(BNBinaryView* data);

	BINARYNINJACOREAPI char* BNGetFilename(BNFileMetadata* file);
	BINARYNINJACOREAPI void BNSetFilename(BNFileMetadata* file, const char* name);

	BINARYNINJACOREAPI void BNRegisterUndoActionType(const char* name, void* typeContext,
	                                                 bool (*deserialize)(void* ctxt, const char* data, BNUndoAction* result));
	BINARYNINJACOREAPI void BNBeginUndoActions(BNFileMetadata* file);
	BINARYNINJACOREAPI void BNAddUndoAction(BNBinaryView* view, const char* type, BNUndoAction* action);
	BINARYNINJACOREAPI void BNCommitUndoActions(BNFileMetadata* file);

	BINARYNINJACOREAPI bool BNUndo(BNFileMetadata* file);
	BINARYNINJACOREAPI bool BNRedo(BNFileMetadata* file);

	BINARYNINJACOREAPI char* BNGetCurrentView(BNFileMetadata* file);
	BINARYNINJACOREAPI uint64_t BNGetCurrentOffset(BNFileMetadata* file);
	BINARYNINJACOREAPI bool BNNavigate(BNFileMetadata* file, const char* view, uint64_t offset);

	BINARYNINJACOREAPI BNBinaryView* BNGetFileViewOfType(BNFileMetadata* file, const char* name);

	// Binary view access
	BINARYNINJACOREAPI BNBinaryView* BNNewViewReference(BNBinaryView* view);
	BINARYNINJACOREAPI void BNFreeBinaryView(BNBinaryView* view);
	BINARYNINJACOREAPI BNFileMetadata* BNGetFileForView(BNBinaryView* view);
	BINARYNINJACOREAPI char* BNGetViewType(BNBinaryView* view);

	BINARYNINJACOREAPI size_t BNReadViewData(BNBinaryView* view, void* dest, uint64_t offset, size_t len);
	BINARYNINJACOREAPI BNDataBuffer* BNReadViewBuffer(BNBinaryView* view, uint64_t offset, size_t len);

	BINARYNINJACOREAPI size_t BNWriteViewData(BNBinaryView* view, uint64_t offset, const void* data, size_t len);
	BINARYNINJACOREAPI size_t BNWriteViewBuffer(BNBinaryView* view, uint64_t offset, BNDataBuffer* data);
	BINARYNINJACOREAPI size_t BNInsertViewData(BNBinaryView* view, uint64_t offset, const void* data, size_t len);
	BINARYNINJACOREAPI size_t BNInsertViewBuffer(BNBinaryView* view, uint64_t offset, BNDataBuffer* data);
	BINARYNINJACOREAPI size_t BNRemoveViewData(BNBinaryView* view, uint64_t offset, uint64_t len);

	BINARYNINJACOREAPI void BNNotifyDataWritten(BNBinaryView* view, uint64_t offset, size_t len);
	BINARYNINJACOREAPI void BNNotifyDataInserted(BNBinaryView* view, uint64_t offset, size_t len);
	BINARYNINJACOREAPI void BNNotifyDataRemoved(BNBinaryView* view, uint64_t offset, uint64_t len);

	BINARYNINJACOREAPI BNModificationStatus BNGetModification(BNBinaryView* view, uint64_t offset);
	BINARYNINJACOREAPI size_t BNGetModificationArray(BNBinaryView* view, uint64_t offset, BNModificationStatus* result, size_t len);

	BINARYNINJACOREAPI bool BNIsValidOffset(BNBinaryView* view, uint64_t offset);
	BINARYNINJACOREAPI bool BNIsOffsetReadable(BNBinaryView* view, uint64_t offset);
	BINARYNINJACOREAPI bool BNIsOffsetWritable(BNBinaryView* view, uint64_t offset);
	BINARYNINJACOREAPI bool BNIsOffsetExecutable(BNBinaryView* view, uint64_t offset);
	BINARYNINJACOREAPI uint64_t BNGetNextValidOffset(BNBinaryView* view, uint64_t offset);
	BINARYNINJACOREAPI uint64_t BNGetStartOffset(BNBinaryView* view);
	BINARYNINJACOREAPI uint64_t BNGetEndOffset(BNBinaryView* view);
	BINARYNINJACOREAPI uint64_t BNGetViewLength(BNBinaryView* view);
	BINARYNINJACOREAPI uint64_t BNGetEntryPoint(BNBinaryView* view);

	BINARYNINJACOREAPI BNArchitecture* BNGetDefaultArchitecture(BNBinaryView* view);
	BINARYNINJACOREAPI void BNSetDefaultArchitecture(BNBinaryView* view, BNArchitecture* arch);
	BINARYNINJACOREAPI BNPlatform* BNGetDefaultPlatform(BNBinaryView* view);
	BINARYNINJACOREAPI void BNSetDefaultPlatform(BNBinaryView* view, BNPlatform* platform);
	BINARYNINJACOREAPI BNEndianness BNGetDefaultEndianness(BNBinaryView* view);
	BINARYNINJACOREAPI size_t BNGetViewAddressSize(BNBinaryView* view);

	BINARYNINJACOREAPI bool BNIsViewModified(BNBinaryView* view);
	BINARYNINJACOREAPI bool BNIsExecutableView(BNBinaryView* view);

	BINARYNINJACOREAPI bool BNSaveToFile(BNBinaryView* view, BNFileAccessor* file);
	BINARYNINJACOREAPI bool BNSaveToFilename(BNBinaryView* view, const char* filename);

	BINARYNINJACOREAPI void BNRegisterDataNotification(BNBinaryView* view, BNBinaryDataNotification* notify);
	BINARYNINJACOREAPI void BNUnregisterDataNotification(BNBinaryView* view, BNBinaryDataNotification* notify);

	BINARYNINJACOREAPI bool BNIsNeverBranchPatchAvailable(BNBinaryView* view, BNArchitecture* arch, uint64_t addr);
	BINARYNINJACOREAPI bool BNIsAlwaysBranchPatchAvailable(BNBinaryView* view, BNArchitecture* arch, uint64_t addr);
	BINARYNINJACOREAPI bool BNIsInvertBranchPatchAvailable(BNBinaryView* view, BNArchitecture* arch, uint64_t addr);
	BINARYNINJACOREAPI bool BNIsSkipAndReturnZeroPatchAvailable(BNBinaryView* view, BNArchitecture* arch, uint64_t addr);
	BINARYNINJACOREAPI bool BNIsSkipAndReturnValuePatchAvailable(BNBinaryView* view, BNArchitecture* arch, uint64_t addr);

	BINARYNINJACOREAPI bool BNConvertToNop(BNBinaryView* view, BNArchitecture* arch, uint64_t addr);
	BINARYNINJACOREAPI bool BNAlwaysBranch(BNBinaryView* view, BNArchitecture* arch, uint64_t addr);
	BINARYNINJACOREAPI bool BNInvertBranch(BNBinaryView* view, BNArchitecture* arch, uint64_t addr);
	BINARYNINJACOREAPI bool BNSkipAndReturnValue(BNBinaryView* view, BNArchitecture* arch, uint64_t addr, uint64_t value);

	BINARYNINJACOREAPI size_t BNGetInstructionLength(BNBinaryView* view, BNArchitecture* arch, uint64_t addr);

	BINARYNINJACOREAPI bool BNFindNextData(BNBinaryView* view, uint64_t start, BNDataBuffer* data, uint64_t* result,
		BNFindFlag flags);

	// Raw binary data view
	BINARYNINJACOREAPI BNBinaryView* BNCreateBinaryDataView(BNFileMetadata* file);
	BINARYNINJACOREAPI BNBinaryView* BNCreateBinaryDataViewFromBuffer(BNFileMetadata* file, BNDataBuffer* buf);
	BINARYNINJACOREAPI BNBinaryView* BNCreateBinaryDataViewFromData(BNFileMetadata* file, const void* data, size_t len);
	BINARYNINJACOREAPI BNBinaryView* BNCreateBinaryDataViewFromFilename(BNFileMetadata* file, const char* filename);
	BINARYNINJACOREAPI BNBinaryView* BNCreateBinaryDataViewFromFile(BNFileMetadata* file, BNFileAccessor* accessor);

	// Creation of new types of binary views
	BINARYNINJACOREAPI BNBinaryView* BNCreateCustomBinaryView(const char* name, BNFileMetadata* file, BNCustomBinaryView* view);

	// Binary view type management
	BINARYNINJACOREAPI BNBinaryViewType* BNGetBinaryViewTypeByName(const char* name);
	BINARYNINJACOREAPI BNBinaryViewType** BNGetBinaryViewTypes(size_t* count);
	BINARYNINJACOREAPI BNBinaryViewType** BNGetBinaryViewTypesForData(BNBinaryView* data, size_t* count);
	BINARYNINJACOREAPI void BNFreeBinaryViewTypeList(BNBinaryViewType** types);
	BINARYNINJACOREAPI char* BNGetBinaryViewTypeName(BNBinaryViewType* type);
	BINARYNINJACOREAPI char* BNGetBinaryViewTypeLongName(BNBinaryViewType* type);
	BINARYNINJACOREAPI BNBinaryView* BNCreateBinaryViewOfType(BNBinaryViewType* type, BNBinaryView* data);
	BINARYNINJACOREAPI bool BNIsBinaryViewTypeValidForData(BNBinaryViewType* type, BNBinaryView* data);

	BINARYNINJACOREAPI BNBinaryViewType* BNRegisterBinaryViewType(const char* name, const char* longName,
	                                                              BNCustomBinaryViewType* type);

	BINARYNINJACOREAPI void BNRegisterArchitectureForViewType(BNBinaryViewType* type, uint32_t id,
		BNEndianness endian, BNArchitecture* arch);
	BINARYNINJACOREAPI BNArchitecture* BNGetArchitectureForViewType(BNBinaryViewType* type, uint32_t id,
		BNEndianness endian);

	BINARYNINJACOREAPI void BNRegisterPlatformForViewType(BNBinaryViewType* type, uint32_t id, BNArchitecture* arch,
	                                                      BNPlatform* platform);
	BINARYNINJACOREAPI void BNRegisterDefaultPlatformForViewType(BNBinaryViewType* type, BNArchitecture* arch,
	                                                             BNPlatform* platform);
	BINARYNINJACOREAPI BNPlatform* BNGetPlatformForViewType(BNBinaryViewType* type, uint32_t id, BNArchitecture* arch);

	// Stream reader object
	BINARYNINJACOREAPI BNBinaryReader* BNCreateBinaryReader(BNBinaryView* view);
	BINARYNINJACOREAPI void BNFreeBinaryReader(BNBinaryReader* stream);
	BINARYNINJACOREAPI BNEndianness BNGetBinaryReaderEndianness(BNBinaryReader* stream);
	BINARYNINJACOREAPI void BNSetBinaryReaderEndianness(BNBinaryReader* stream, BNEndianness endian);

	BINARYNINJACOREAPI bool BNReadData(BNBinaryReader* stream, void* dest, size_t len);
	BINARYNINJACOREAPI bool BNRead8(BNBinaryReader* stream, uint8_t* result);
	BINARYNINJACOREAPI bool BNRead16(BNBinaryReader* stream, uint16_t* result);
	BINARYNINJACOREAPI bool BNRead32(BNBinaryReader* stream, uint32_t* result);
	BINARYNINJACOREAPI bool BNRead64(BNBinaryReader* stream, uint64_t* result);
	BINARYNINJACOREAPI bool BNReadLE16(BNBinaryReader* stream, uint16_t* result);
	BINARYNINJACOREAPI bool BNReadLE32(BNBinaryReader* stream, uint32_t* result);
	BINARYNINJACOREAPI bool BNReadLE64(BNBinaryReader* stream, uint64_t* result);
	BINARYNINJACOREAPI bool BNReadBE16(BNBinaryReader* stream, uint16_t* result);
	BINARYNINJACOREAPI bool BNReadBE32(BNBinaryReader* stream, uint32_t* result);
	BINARYNINJACOREAPI bool BNReadBE64(BNBinaryReader* stream, uint64_t* result);

	BINARYNINJACOREAPI uint64_t BNGetReaderPosition(BNBinaryReader* stream);
	BINARYNINJACOREAPI void BNSeekBinaryReader(BNBinaryReader* stream, uint64_t offset);
	BINARYNINJACOREAPI void BNSeekBinaryReaderRelative(BNBinaryReader* stream, int64_t offset);
	BINARYNINJACOREAPI bool BNIsEndOfFile(BNBinaryReader* stream);

	// Stream writer object
	BINARYNINJACOREAPI BNBinaryWriter* BNCreateBinaryWriter(BNBinaryView* view);
	BINARYNINJACOREAPI void BNFreeBinaryWriter(BNBinaryWriter* stream);
	BINARYNINJACOREAPI BNEndianness BNGetBinaryWriterEndianness(BNBinaryWriter* stream);
	BINARYNINJACOREAPI void BNSetBinaryWriterEndianness(BNBinaryWriter* stream, BNEndianness endian);

	BINARYNINJACOREAPI bool BNWriteData(BNBinaryWriter* stream, const void* src, size_t len);
	BINARYNINJACOREAPI bool BNWrite8(BNBinaryWriter* stream, uint8_t val);
	BINARYNINJACOREAPI bool BNWrite16(BNBinaryWriter* stream, uint16_t val);
	BINARYNINJACOREAPI bool BNWrite32(BNBinaryWriter* stream, uint32_t val);
	BINARYNINJACOREAPI bool BNWrite64(BNBinaryWriter* stream, uint64_t val);
	BINARYNINJACOREAPI bool BNWriteLE16(BNBinaryWriter* stream, uint16_t val);
	BINARYNINJACOREAPI bool BNWriteLE32(BNBinaryWriter* stream, uint32_t val);
	BINARYNINJACOREAPI bool BNWriteLE64(BNBinaryWriter* stream, uint64_t val);
	BINARYNINJACOREAPI bool BNWriteBE16(BNBinaryWriter* stream, uint16_t val);
	BINARYNINJACOREAPI bool BNWriteBE32(BNBinaryWriter* stream, uint32_t val);
	BINARYNINJACOREAPI bool BNWriteBE64(BNBinaryWriter* stream, uint64_t val);

	BINARYNINJACOREAPI uint64_t BNGetWriterPosition(BNBinaryWriter* stream);
	BINARYNINJACOREAPI void BNSeekBinaryWriter(BNBinaryWriter* stream, uint64_t offset);
	BINARYNINJACOREAPI void BNSeekBinaryWriterRelative(BNBinaryWriter* stream, int64_t offset);

	// Transforms
	BINARYNINJACOREAPI BNTransform* BNGetTransformByName(const char* name);
	BINARYNINJACOREAPI BNTransform** BNGetTransformTypeList(size_t* count);
	BINARYNINJACOREAPI void BNFreeTransformTypeList(BNTransform** xforms);
	BINARYNINJACOREAPI BNTransform* BNRegisterTransformType(BNTransformType type, const char* name, const char* longName,
	                                                        const char* group, BNCustomTransform* xform);

	BINARYNINJACOREAPI BNTransformType BNGetTransformType(BNTransform* xform);
	BINARYNINJACOREAPI char* BNGetTransformName(BNTransform* xform);
	BINARYNINJACOREAPI char* BNGetTransformLongName(BNTransform* xform);
	BINARYNINJACOREAPI char* BNGetTransformGroup(BNTransform* xform);
	BINARYNINJACOREAPI BNTransformParameterInfo* BNGetTransformParameterList(BNTransform* xform, size_t* count);
	BINARYNINJACOREAPI void BNFreeTransformParameterList(BNTransformParameterInfo* params, size_t count);
	BINARYNINJACOREAPI bool BNDecode(BNTransform* xform, BNDataBuffer* input, BNDataBuffer* output,
	                                 BNTransformParameter* params, size_t paramCount);
	BINARYNINJACOREAPI bool BNEncode(BNTransform* xform, BNDataBuffer* input, BNDataBuffer* output,
	                                 BNTransformParameter* params, size_t paramCount);

	// Architectures
	BINARYNINJACOREAPI BNArchitecture* BNGetArchitectureByName(const char* name);
	BINARYNINJACOREAPI BNArchitecture** BNGetArchitectureList(size_t* count);
	BINARYNINJACOREAPI void BNFreeArchitectureList(BNArchitecture** archs);
	BINARYNINJACOREAPI BNArchitecture* BNRegisterArchitecture(const char* name, BNCustomArchitecture* arch);

	BINARYNINJACOREAPI char* BNGetArchitectureName(BNArchitecture* arch);
	BINARYNINJACOREAPI BNEndianness BNGetArchitectureEndianness(BNArchitecture* arch);
	BINARYNINJACOREAPI size_t BNGetArchitectureAddressSize(BNArchitecture* arch);
	BINARYNINJACOREAPI size_t BNGetArchitectureDefaultIntegerSize(BNArchitecture* arch);
	BINARYNINJACOREAPI size_t BNGetArchitectureMaxInstructionLength(BNArchitecture* arch);
	BINARYNINJACOREAPI size_t BNGetArchitectureOpcodeDisplayLength(BNArchitecture* arch);
	BINARYNINJACOREAPI bool BNGetInstructionInfo(BNArchitecture* arch, const uint8_t* data, uint64_t addr,
	                                             size_t maxLen, BNInstructionInfo* result);
	BINARYNINJACOREAPI bool BNGetInstructionText(BNArchitecture* arch, const uint8_t* data, uint64_t addr,
	                                             size_t* len, BNInstructionTextToken** result, size_t* count);
	BINARYNINJACOREAPI bool BNGetInstructionLowLevelIL(BNArchitecture* arch, const uint8_t* data, uint64_t addr,
	                                                   size_t* len, BNLowLevelILFunction* il);
	BINARYNINJACOREAPI void BNFreeInstructionText(BNInstructionTextToken* tokens, size_t count);
	BINARYNINJACOREAPI char* BNGetArchitectureRegisterName(BNArchitecture* arch, uint32_t reg);
	BINARYNINJACOREAPI char* BNGetArchitectureFlagName(BNArchitecture* arch, uint32_t flag);
	BINARYNINJACOREAPI char* BNGetArchitectureFlagWriteTypeName(BNArchitecture* arch, uint32_t flags);
	BINARYNINJACOREAPI uint32_t* BNGetFullWidthArchitectureRegisters(BNArchitecture* arch, size_t* count);
	BINARYNINJACOREAPI uint32_t* BNGetAllArchitectureRegisters(BNArchitecture* arch, size_t* count);
	BINARYNINJACOREAPI uint32_t* BNGetAllArchitectureFlags(BNArchitecture* arch, size_t* count);
	BINARYNINJACOREAPI uint32_t* BNGetAllArchitectureFlagWriteTypes(BNArchitecture* arch, size_t* count);
	BINARYNINJACOREAPI BNFlagRole BNGetArchitectureFlagRole(BNArchitecture* arch, uint32_t flag);
	BINARYNINJACOREAPI uint32_t* BNGetArchitectureFlagsRequiredForFlagCondition(BNArchitecture* arch, BNLowLevelILFlagCondition cond,
		size_t* count);
	BINARYNINJACOREAPI uint32_t* BNGetArchitectureFlagsWrittenByFlagWriteType(BNArchitecture* arch, uint32_t writeType,
		size_t* count);
	BINARYNINJACOREAPI size_t BNGetArchitectureFlagWriteLowLevelIL(BNArchitecture* arch, BNLowLevelILOperation op,
		size_t size, uint32_t flagWriteType, uint32_t flag, BNRegisterOrConstant* operands, size_t operandCount,
		BNLowLevelILFunction* il);
	BINARYNINJACOREAPI size_t BNGetDefaultArchitectureFlagWriteLowLevelIL(BNArchitecture* arch, BNLowLevelILOperation op,
		size_t size, uint32_t flagWriteType, uint32_t flag, BNRegisterOrConstant* operands, size_t operandCount,
		BNLowLevelILFunction* il);
	BINARYNINJACOREAPI size_t BNGetArchitectureFlagConditionLowLevelIL(BNArchitecture* arch, BNLowLevelILFlagCondition cond,
		BNLowLevelILFunction* il);
	BINARYNINJACOREAPI uint32_t* BNGetModifiedArchitectureRegistersOnWrite(BNArchitecture* arch, uint32_t reg, size_t* count);
	BINARYNINJACOREAPI void BNFreeRegisterList(uint32_t* regs);
	BINARYNINJACOREAPI BNRegisterInfo BNGetArchitectureRegisterInfo(BNArchitecture* arch, uint32_t reg);
	BINARYNINJACOREAPI uint32_t BNGetArchitectureStackPointerRegister(BNArchitecture* arch);
	BINARYNINJACOREAPI uint32_t BNGetArchitectureLinkRegister(BNArchitecture* arch);
	BINARYNINJACOREAPI uint32_t BNGetArchitectureRegisterByName(BNArchitecture* arch, const char* name);

	BINARYNINJACOREAPI bool BNAssemble(BNArchitecture* arch, const char* code, uint64_t addr, BNDataBuffer* result, char** errors);

	BINARYNINJACOREAPI bool BNIsArchitectureNeverBranchPatchAvailable(BNArchitecture* arch, const uint8_t* data,
	                                                                  uint64_t addr, size_t len);
	BINARYNINJACOREAPI bool BNIsArchitectureAlwaysBranchPatchAvailable(BNArchitecture* arch, const uint8_t* data,
	                                                                   uint64_t addr, size_t len);
	BINARYNINJACOREAPI bool BNIsArchitectureInvertBranchPatchAvailable(BNArchitecture* arch, const uint8_t* data,
	                                                                   uint64_t addr, size_t len);
	BINARYNINJACOREAPI bool BNIsArchitectureSkipAndReturnZeroPatchAvailable(BNArchitecture* arch, const uint8_t* data,
	                                                                        uint64_t addr, size_t len);
	BINARYNINJACOREAPI bool BNIsArchitectureSkipAndReturnValuePatchAvailable(BNArchitecture* arch, const uint8_t* data,
	                                                                         uint64_t addr, size_t len);

	BINARYNINJACOREAPI bool BNArchitectureConvertToNop(BNArchitecture* arch, uint8_t* data, uint64_t addr, size_t len);
	BINARYNINJACOREAPI bool BNArchitectureAlwaysBranch(BNArchitecture* arch, uint8_t* data, uint64_t addr, size_t len);
	BINARYNINJACOREAPI bool BNArchitectureInvertBranch(BNArchitecture* arch, uint8_t* data, uint64_t addr, size_t len);
	BINARYNINJACOREAPI bool BNArchitectureSkipAndReturnValue(BNArchitecture* arch, uint8_t* data, uint64_t addr,
	                                                         size_t len, uint64_t value);

	BINARYNINJACOREAPI void BNRegisterArchitectureFunctionRecognizer(BNArchitecture* arch, BNFunctionRecognizer* rec);

	BINARYNINJACOREAPI bool BNIsBinaryViewTypeArchitectureConstantDefined(BNArchitecture* arch, const char* type,
	                                                                      const char* name);
	BINARYNINJACOREAPI uint64_t BNGetBinaryViewTypeArchitectureConstant(BNArchitecture* arch, const char* type,
	                                                                    const char* name, uint64_t defaultValue);
	BINARYNINJACOREAPI void BNSetBinaryViewTypeArchitectureConstant(BNArchitecture* arch, const char* type,
	                                                                const char* name, uint64_t value);

	// Analysis
	BINARYNINJACOREAPI void BNAddFunctionForAnalysis(BNBinaryView* view, BNPlatform* platform, uint64_t addr);
	BINARYNINJACOREAPI void BNAddEntryPointForAnalysis(BNBinaryView* view, BNPlatform* platform, uint64_t addr);
	BINARYNINJACOREAPI void BNRemoveAnalysisFunction(BNBinaryView* view, BNFunction* func);
	BINARYNINJACOREAPI void BNCreateUserFunction(BNBinaryView* view, BNPlatform* platform, uint64_t addr);
	BINARYNINJACOREAPI void BNRemoveUserFunction(BNBinaryView* view, BNFunction* func);
	BINARYNINJACOREAPI void BNUpdateAnalysis(BNBinaryView* view);
	BINARYNINJACOREAPI void BNAbortAnalysis(BNBinaryView* view);

	BINARYNINJACOREAPI BNFunction* BNNewFunctionReference(BNFunction* func);
	BINARYNINJACOREAPI void BNFreeFunction(BNFunction* func);
	BINARYNINJACOREAPI BNFunction** BNGetAnalysisFunctionList(BNBinaryView* view, size_t* count);
	BINARYNINJACOREAPI void BNFreeFunctionList(BNFunction** funcs, size_t count);
	BINARYNINJACOREAPI bool BNHasFunctions(BNBinaryView* view);
	BINARYNINJACOREAPI BNFunction* BNGetAnalysisFunction(BNBinaryView* view, BNPlatform* platform, uint64_t addr);
	BINARYNINJACOREAPI BNFunction* BNGetRecentAnalysisFunctionForAddress(BNBinaryView* view, uint64_t addr);
	BINARYNINJACOREAPI BNFunction** BNGetAnalysisFunctionsForAddress(BNBinaryView* view, uint64_t addr, size_t* count);
	BINARYNINJACOREAPI BNFunction* BNGetAnalysisEntryPoint(BNBinaryView* view);

	BINARYNINJACOREAPI BNBinaryView* BNGetFunctionData(BNFunction* func);
	BINARYNINJACOREAPI BNArchitecture* BNGetFunctionArchitecture(BNFunction* func);
	BINARYNINJACOREAPI BNPlatform* BNGetFunctionPlatform(BNFunction* func);
	BINARYNINJACOREAPI uint64_t BNGetFunctionStart(BNFunction* func);
	BINARYNINJACOREAPI BNSymbol* BNGetFunctionSymbol(BNFunction* func);
	BINARYNINJACOREAPI bool BNWasFunctionAutomaticallyDiscovered(BNFunction* func);
	BINARYNINJACOREAPI bool BNCanFunctionReturn(BNFunction* func);
	BINARYNINJACOREAPI void BNSetFunctionAutoType(BNFunction* func, BNType* type);
	BINARYNINJACOREAPI void BNSetFunctionUserType(BNFunction* func, BNType* type);

	BINARYNINJACOREAPI char* BNGetCommentForAddress(BNFunction* func, uint64_t addr);
	BINARYNINJACOREAPI uint64_t* BNGetCommentedAddresses(BNFunction* func, size_t* count);
	BINARYNINJACOREAPI void BNFreeAddressList(uint64_t* addrs);
	BINARYNINJACOREAPI void BNSetCommentForAddress(BNFunction* func, uint64_t addr, const char* comment);

	BINARYNINJACOREAPI BNBasicBlock* BNNewBasicBlockReference(BNBasicBlock* block);
	BINARYNINJACOREAPI void BNFreeBasicBlock(BNBasicBlock* block);
	BINARYNINJACOREAPI BNBasicBlock** BNGetFunctionBasicBlockList(BNFunction* func, size_t* count);
	BINARYNINJACOREAPI void BNFreeBasicBlockList(BNBasicBlock** blocks, size_t count);
	BINARYNINJACOREAPI BNBasicBlock* BNGetRecentBasicBlockForAddress(BNBinaryView* view, uint64_t addr);
	BINARYNINJACOREAPI BNBasicBlock** BNGetBasicBlocksForAddress(BNBinaryView* view, uint64_t addr, size_t* count);

	BINARYNINJACOREAPI BNLowLevelILFunction* BNGetFunctionLowLevelIL(BNFunction* func);
	BINARYNINJACOREAPI size_t BNGetLowLevelILForInstruction(BNFunction* func, BNArchitecture* arch, uint64_t addr);
	BINARYNINJACOREAPI size_t* BNGetLowLevelILExitsForInstruction(BNFunction* func, BNArchitecture* arch, uint64_t addr,
	                                                              size_t* count);
	BINARYNINJACOREAPI void BNFreeLowLevelILInstructionList(size_t* list);
	BINARYNINJACOREAPI BNRegisterValue BNGetRegisterValueAtInstruction(BNFunction* func, BNArchitecture* arch,
		uint64_t addr, uint32_t reg);
	BINARYNINJACOREAPI BNRegisterValue BNGetRegisterValueAfterInstruction(BNFunction* func, BNArchitecture* arch,
		uint64_t addr, uint32_t reg);
	BINARYNINJACOREAPI BNRegisterValue BNGetRegisterValueAtLowLevelILInstruction(BNFunction* func, size_t i, uint32_t reg);
	BINARYNINJACOREAPI BNRegisterValue BNGetRegisterValueAfterLowLevelILInstruction(BNFunction* func, size_t i, uint32_t reg);
	BINARYNINJACOREAPI BNRegisterValue BNGetStackContentsAtInstruction(BNFunction* func, BNArchitecture* arch,
		uint64_t addr, int64_t offset, size_t size);
	BINARYNINJACOREAPI BNRegisterValue BNGetStackContentsAfterInstruction(BNFunction* func, BNArchitecture* arch,
		uint64_t addr, int64_t offset, size_t size);
	BINARYNINJACOREAPI BNRegisterValue BNGetStackContentsAtLowLevelILInstruction(BNFunction* func, size_t i,
		int64_t offset, size_t size);
	BINARYNINJACOREAPI BNRegisterValue BNGetStackContentsAfterLowLevelILInstruction(BNFunction* func, size_t i,
		int64_t offset, size_t size);
	BINARYNINJACOREAPI BNRegisterValue BNGetParameterValueAtInstruction(BNFunction* func, BNArchitecture* arch,
		uint64_t addr, BNType* functionType, size_t i);
	BINARYNINJACOREAPI BNRegisterValue BNGetParameterValueAtLowLevelILInstruction(BNFunction* func, size_t instr,
		BNType* functionType, size_t i);
	BINARYNINJACOREAPI void BNFreeRegisterValue(BNRegisterValue* value);
	BINARYNINJACOREAPI uint32_t* BNGetRegistersReadByInstruction(BNFunction* func, BNArchitecture* arch, uint64_t addr,
	                                                             size_t* count);
	BINARYNINJACOREAPI uint32_t* BNGetRegistersWrittenByInstruction(BNFunction* func, BNArchitecture* arch, uint64_t addr,
	                                                                size_t* count);
	BINARYNINJACOREAPI BNStackVariableReference* BNGetStackVariablesReferencedByInstruction(BNFunction* func, BNArchitecture* arch,
	                                                                                        uint64_t addr, size_t* count);
	BINARYNINJACOREAPI void BNFreeStackVariableReferenceList(BNStackVariableReference* refs, size_t count);

	BINARYNINJACOREAPI BNLowLevelILFunction* BNGetFunctionLiftedIL(BNFunction* func);
	BINARYNINJACOREAPI size_t BNGetLiftedILForInstruction(BNFunction* func, BNArchitecture* arch, uint64_t addr);
	BINARYNINJACOREAPI size_t* BNGetLiftedILFlagUsesForDefinition(BNFunction* func, size_t i, uint32_t flag, size_t* count);
	BINARYNINJACOREAPI size_t* BNGetLiftedILFlagDefinitionsForUse(BNFunction* func, size_t i, uint32_t flag, size_t* count);
	BINARYNINJACOREAPI uint32_t* BNGetFlagsReadByLiftedILInstruction(BNFunction* func, size_t i, size_t* count);
	BINARYNINJACOREAPI uint32_t* BNGetFlagsWrittenByLiftedILInstruction(BNFunction* func, size_t i, size_t* count);

	BINARYNINJACOREAPI BNType* BNGetFunctionType(BNFunction* func);
	BINARYNINJACOREAPI void BNApplyImportedTypes(BNFunction* func, BNSymbol* sym);
	BINARYNINJACOREAPI void BNApplyAutoDiscoveredFunctionType(BNFunction* func, BNType* type);
	BINARYNINJACOREAPI bool BNFunctionHasExplicitlyDefinedType(BNFunction* func);

	BINARYNINJACOREAPI BNFunction* BNGetBasicBlockFunction(BNBasicBlock* block);
	BINARYNINJACOREAPI BNArchitecture* BNGetBasicBlockArchitecture(BNBasicBlock* block);
	BINARYNINJACOREAPI uint64_t BNGetBasicBlockStart(BNBasicBlock* block);
	BINARYNINJACOREAPI uint64_t BNGetBasicBlockEnd(BNBasicBlock* block);
	BINARYNINJACOREAPI uint64_t BNGetBasicBlockLength(BNBasicBlock* block);
	BINARYNINJACOREAPI BNBasicBlockEdge* BNGetBasicBlockOutgoingEdges(BNBasicBlock* block, size_t* count);
	BINARYNINJACOREAPI void BNFreeBasicBlockOutgoingEdgeList(BNBasicBlockEdge* edges);
	BINARYNINJACOREAPI bool BNBasicBlockHasUndeterminedOutgoingEdges(BNBasicBlock* block);

	BINARYNINJACOREAPI BNDisassemblyTextLine* BNGetBasicBlockDisassemblyText(BNBasicBlock* block,
		BNDisassemblySettings* settings, size_t* count);
	BINARYNINJACOREAPI void BNFreeDisassemblyTextLines(BNDisassemblyTextLine* lines, size_t count);

	BINARYNINJACOREAPI void BNMarkFunctionAsRecentlyUsed(BNFunction* func);
	BINARYNINJACOREAPI void BNMarkBasicBlockAsRecentlyUsed(BNBasicBlock* block);

	BINARYNINJACOREAPI BNReferenceSource* BNGetCodeReferences(BNBinaryView* view, uint64_t addr, size_t* count);
	BINARYNINJACOREAPI BNReferenceSource* BNGetCodeReferencesInRange(BNBinaryView* view, uint64_t addr,
	                                                                 uint64_t len, size_t* count);
	BINARYNINJACOREAPI void BNFreeCodeReferences(BNReferenceSource* refs, size_t count);

	BINARYNINJACOREAPI void BNRegisterGlobalFunctionRecognizer(BNFunctionRecognizer* rec);

	BINARYNINJACOREAPI BNStringReference* BNGetStrings(BNBinaryView* view, size_t* count);
	BINARYNINJACOREAPI BNStringReference* BNGetStringsInRange(BNBinaryView* view, uint64_t start,
	                                                          uint64_t len, size_t* count);
	BINARYNINJACOREAPI void BNFreeStringList(BNStringReference* strings);

	BINARYNINJACOREAPI BNStackVariable* BNGetStackLayout(BNFunction* func, size_t* count);
	BINARYNINJACOREAPI void BNFreeStackLayout(BNStackVariable* vars, size_t count);
	BINARYNINJACOREAPI void BNCreateAutoStackVariable(BNFunction* func, int64_t offset, BNType* type, const char* name);
	BINARYNINJACOREAPI void BNCreateUserStackVariable(BNFunction* func, int64_t offset, BNType* type, const char* name);
	BINARYNINJACOREAPI void BNDeleteAutoStackVariable(BNFunction* func, int64_t offset);
	BINARYNINJACOREAPI void BNDeleteUserStackVariable(BNFunction* func, int64_t offset);
	BINARYNINJACOREAPI bool BNGetStackVariableAtFrameOffset(BNFunction* func, int64_t offset, BNStackVariable* var);
	BINARYNINJACOREAPI void BNFreeStackVariable(BNStackVariable* var);

	BINARYNINJACOREAPI void BNSetAutoIndirectBranches(BNFunction* func, BNArchitecture* sourceArch, uint64_t source,
	                                                  BNArchitectureAndAddress* branches, size_t count);
	BINARYNINJACOREAPI void BNSetUserIndirectBranches(BNFunction* func, BNArchitecture* sourceArch, uint64_t source,
	                                                  BNArchitectureAndAddress* branches, size_t count);

	BINARYNINJACOREAPI BNIndirectBranchInfo* BNGetIndirectBranches(BNFunction* func, size_t* count);
	BINARYNINJACOREAPI BNIndirectBranchInfo* BNGetIndirectBranchesAt(BNFunction* func, BNArchitecture* arch,
	                                                                 uint64_t addr, size_t* count);
	BINARYNINJACOREAPI void BNFreeIndirectBranchList(BNIndirectBranchInfo* branches);

	BINARYNINJACOREAPI BNInstructionTextLine* BNGetFunctionBlockAnnotations(BNFunction* func, BNArchitecture* arch,
		uint64_t addr, size_t* count);
	BINARYNINJACOREAPI void BNFreeInstructionTextLines(BNInstructionTextLine* lines, size_t count);

	BINARYNINJACOREAPI BNIntegerDisplayType BNGetIntegerConstantDisplayType(BNFunction* func, BNArchitecture* arch,
		uint64_t instrAddr, uint64_t value, size_t operand);
	BINARYNINJACOREAPI void BNSetIntegerConstantDisplayType(BNFunction* func, BNArchitecture* arch,
		uint64_t instrAddr, uint64_t value, size_t operand, BNIntegerDisplayType type);

	BINARYNINJACOREAPI BNAnalysisCompletionEvent* BNAddAnalysisCompletionEvent(BNBinaryView* view, void* ctxt,
		void (*callback)(void* ctxt));
	BINARYNINJACOREAPI BNAnalysisCompletionEvent* BNNewAnalysisCompletionEventReference(BNAnalysisCompletionEvent* event);
	BINARYNINJACOREAPI void BNFreeAnalysisCompletionEvent(BNAnalysisCompletionEvent* event);
	BINARYNINJACOREAPI void BNCancelAnalysisCompletionEvent(BNAnalysisCompletionEvent* event);

	BINARYNINJACOREAPI BNAnalysisProgress BNGetAnalysisProgress(BNBinaryView* view);

	BINARYNINJACOREAPI uint64_t BNGetNextFunctionStartAfterAddress(BNBinaryView* view, uint64_t addr);
	BINARYNINJACOREAPI uint64_t BNGetNextBasicBlockStartAfterAddress(BNBinaryView* view, uint64_t addr);
	BINARYNINJACOREAPI uint64_t BNGetNextDataAfterAddress(BNBinaryView* view, uint64_t addr);
	BINARYNINJACOREAPI uint64_t BNGetNextDataVariableAfterAddress(BNBinaryView* view, uint64_t addr);
	BINARYNINJACOREAPI uint64_t BNGetPreviousFunctionStartBeforeAddress(BNBinaryView* view, uint64_t addr);
	BINARYNINJACOREAPI uint64_t BNGetPreviousBasicBlockStartBeforeAddress(BNBinaryView* view, uint64_t addr);
	BINARYNINJACOREAPI uint64_t BNGetPreviousBasicBlockEndBeforeAddress(BNBinaryView* view, uint64_t addr);
	BINARYNINJACOREAPI uint64_t BNGetPreviousDataBeforeAddress(BNBinaryView* view, uint64_t addr);
	BINARYNINJACOREAPI uint64_t BNGetPreviousDataVariableBeforeAddress(BNBinaryView* view, uint64_t addr);

	BINARYNINJACOREAPI BNLinearDisassemblyPosition BNGetLinearDisassemblyPositionForAddress(BNBinaryView* view,
		uint64_t addr, BNDisassemblySettings* settings);
	BINARYNINJACOREAPI void BNFreeLinearDisassemblyPosition(BNLinearDisassemblyPosition* pos);
	BINARYNINJACOREAPI BNLinearDisassemblyLine* BNGetPreviousLinearDisassemblyLines(BNBinaryView* view,
		BNLinearDisassemblyPosition* pos, BNDisassemblySettings* settings, size_t* count);
	BINARYNINJACOREAPI BNLinearDisassemblyLine* BNGetNextLinearDisassemblyLines(BNBinaryView* view,
		BNLinearDisassemblyPosition* pos, BNDisassemblySettings* settings, size_t* count);
	BINARYNINJACOREAPI void BNFreeLinearDisassemblyLines(BNLinearDisassemblyLine* lines, size_t count);

	BINARYNINJACOREAPI void BNDefineDataVariable(BNBinaryView* view, uint64_t addr, BNType* type);
	BINARYNINJACOREAPI void BNDefineUserDataVariable(BNBinaryView* view, uint64_t addr, BNType* type);
	BINARYNINJACOREAPI void BNUndefineDataVariable(BNBinaryView* view, uint64_t addr);
	BINARYNINJACOREAPI void BNUndefineUserDataVariable(BNBinaryView* view, uint64_t addr);
	BINARYNINJACOREAPI BNDataVariable* BNGetDataVariables(BNBinaryView* view, size_t* count);
	BINARYNINJACOREAPI void BNFreeDataVariables(BNDataVariable* vars, size_t count);
	BINARYNINJACOREAPI bool BNGetDataVariableAtAddress(BNBinaryView* view, uint64_t addr, BNDataVariable* var);

	BINARYNINJACOREAPI bool BNParseTypeString(BNBinaryView* view, const char* text, BNNameAndType* result, char** errors);
	BINARYNINJACOREAPI void BNFreeNameAndType(BNNameAndType* obj);

	BINARYNINJACOREAPI BNNameAndType* BNGetAnalysisTypeList(BNBinaryView* view, size_t* count);
	BINARYNINJACOREAPI void BNFreeTypeList(BNNameAndType* types, size_t count);
	BINARYNINJACOREAPI BNType* BNGetAnalysisTypeByName(BNBinaryView* view, const char* name);
	BINARYNINJACOREAPI bool BNIsAnalysisTypeAutoDefined(BNBinaryView* view, const char* name);
	BINARYNINJACOREAPI void BNDefineAnalysisType(BNBinaryView* view, const char* name, BNType* type);
	BINARYNINJACOREAPI void BNDefineUserAnalysisType(BNBinaryView* view, const char* name, BNType* type);
	BINARYNINJACOREAPI void BNUndefineAnalysisType(BNBinaryView* view, const char* name);
	BINARYNINJACOREAPI void BNUndefineUserAnalysisType(BNBinaryView* view, const char* name);

	// Disassembly settings
	BINARYNINJACOREAPI BNDisassemblySettings* BNCreateDisassemblySettings(void);
	BINARYNINJACOREAPI BNDisassemblySettings* BNNewDisassemblySettingsReference(BNDisassemblySettings* settings);
	BINARYNINJACOREAPI void BNFreeDisassemblySettings(BNDisassemblySettings* settings);

	BINARYNINJACOREAPI bool BNIsDisassemblySettingsOptionSet(BNDisassemblySettings* settings,
		BNDisassemblyOption option);
	BINARYNINJACOREAPI void BNSetDisassemblySettingsOption(BNDisassemblySettings* settings,
		BNDisassemblyOption option, bool state);

	BINARYNINJACOREAPI size_t BNGetDisassemblyWidth(BNDisassemblySettings* settings);
	BINARYNINJACOREAPI void BNSetDisassemblyWidth(BNDisassemblySettings* settings, size_t width);
	BINARYNINJACOREAPI size_t BNGetDisassemblyMaximumSymbolWidth(BNDisassemblySettings* settings);
	BINARYNINJACOREAPI void BNSetDisassemblyMaximumSymbolWidth(BNDisassemblySettings* settings, size_t width);

	// Function graph
	BINARYNINJACOREAPI BNFunctionGraph* BNCreateFunctionGraph(BNFunction* func);
	BINARYNINJACOREAPI BNFunctionGraph* BNNewFunctionGraphReference(BNFunctionGraph* graph);
	BINARYNINJACOREAPI void BNFreeFunctionGraph(BNFunctionGraph* graph);
	BINARYNINJACOREAPI BNFunction* BNGetFunctionForFunctionGraph(BNFunctionGraph* graph);

	BINARYNINJACOREAPI int BNGetHorizontalFunctionGraphBlockMargin(BNFunctionGraph* graph);
	BINARYNINJACOREAPI int BNGetVerticalFunctionGraphBlockMargin(BNFunctionGraph* graph);
	BINARYNINJACOREAPI void BNSetFunctionGraphBlockMargins(BNFunctionGraph* graph, int horiz, int vert);

	BINARYNINJACOREAPI BNDisassemblySettings* BNGetFunctionGraphSettings(BNFunctionGraph* graph);

	BINARYNINJACOREAPI void BNStartFunctionGraphLayout(BNFunctionGraph* graph, BNFunctionGraphType type);
	BINARYNINJACOREAPI bool BNIsFunctionGraphLayoutComplete(BNFunctionGraph* graph);
	BINARYNINJACOREAPI void BNSetFunctionGraphCompleteCallback(BNFunctionGraph* graph, void* ctxt, void (*func)(void* ctxt));
	BINARYNINJACOREAPI void BNAbortFunctionGraph(BNFunctionGraph* graph);
	BINARYNINJACOREAPI BNFunctionGraphType BNGetFunctionGraphType(BNFunctionGraph* graph);

	BINARYNINJACOREAPI BNFunctionGraphBlock** BNGetFunctionGraphBlocks(BNFunctionGraph* graph, size_t* count);
	BINARYNINJACOREAPI BNFunctionGraphBlock** BNGetFunctionGraphBlocksInRegion(
		BNFunctionGraph* graph, int left, int top, int right, int bottom, size_t* count);
	BINARYNINJACOREAPI void BNFreeFunctionGraphBlockList(BNFunctionGraphBlock** blocks, size_t count);

	BINARYNINJACOREAPI int BNGetFunctionGraphWidth(BNFunctionGraph* graph);
	BINARYNINJACOREAPI int BNGetFunctionGraphHeight(BNFunctionGraph* graph);

	BINARYNINJACOREAPI bool BNIsFunctionGraphOptionSet(BNFunctionGraph* graph, BNDisassemblyOption option);
	BINARYNINJACOREAPI void BNSetFunctionGraphOption(BNFunctionGraph* graph, BNDisassemblyOption option, bool state);

	BINARYNINJACOREAPI BNFunctionGraphBlock* BNNewFunctionGraphBlockReference(BNFunctionGraphBlock* block);
	BINARYNINJACOREAPI void BNFreeFunctionGraphBlock(BNFunctionGraphBlock* block);

	BINARYNINJACOREAPI BNArchitecture* BNGetFunctionGraphBlockArchitecture(BNFunctionGraphBlock* block);
	BINARYNINJACOREAPI uint64_t BNGetFunctionGraphBlockStart(BNFunctionGraphBlock* block);
	BINARYNINJACOREAPI uint64_t BNGetFunctionGraphBlockEnd(BNFunctionGraphBlock* block);
	BINARYNINJACOREAPI int BNGetFunctionGraphBlockX(BNFunctionGraphBlock* block);
	BINARYNINJACOREAPI int BNGetFunctionGraphBlockY(BNFunctionGraphBlock* block);
	BINARYNINJACOREAPI int BNGetFunctionGraphBlockWidth(BNFunctionGraphBlock* block);
	BINARYNINJACOREAPI int BNGetFunctionGraphBlockHeight(BNFunctionGraphBlock* block);

	BINARYNINJACOREAPI BNDisassemblyTextLine* BNGetFunctionGraphBlockLines(BNFunctionGraphBlock* block, size_t* count);
	BINARYNINJACOREAPI BNFunctionGraphEdge* BNGetFunctionGraphBlockOutgoingEdges(BNFunctionGraphBlock* block, size_t* count);
	BINARYNINJACOREAPI void BNFreeFunctionGraphBlockOutgoingEdgeList(BNFunctionGraphEdge* edges, size_t count);

	// Symbols
	BINARYNINJACOREAPI BNSymbol* BNCreateSymbol(BNSymbolType type, const char* shortName, const char* fullName,
	                                            const char* rawName, uint64_t addr);
	BINARYNINJACOREAPI BNSymbol* BNNewSymbolReference(BNSymbol* sym);
	BINARYNINJACOREAPI void BNFreeSymbol(BNSymbol* sym);
	BINARYNINJACOREAPI BNSymbolType BNGetSymbolType(BNSymbol* sym);
	BINARYNINJACOREAPI char* BNGetSymbolShortName(BNSymbol* sym);
	BINARYNINJACOREAPI char* BNGetSymbolFullName(BNSymbol* sym);
	BINARYNINJACOREAPI char* BNGetSymbolRawName(BNSymbol* sym);
	BINARYNINJACOREAPI uint64_t BNGetSymbolAddress(BNSymbol* sym);
	BINARYNINJACOREAPI bool BNIsSymbolAutoDefined(BNSymbol* sym);
	BINARYNINJACOREAPI void BNSetSymbolAutoDefined(BNSymbol* sym, bool val);

	BINARYNINJACOREAPI BNSymbol* BNGetSymbolByAddress(BNBinaryView* view, uint64_t addr);
	BINARYNINJACOREAPI BNSymbol* BNGetSymbolByRawName(BNBinaryView* view, const char* name);
	BINARYNINJACOREAPI BNSymbol** BNGetSymbolsByName(BNBinaryView* view, const char* name, size_t* count);
	BINARYNINJACOREAPI BNSymbol** BNGetSymbols(BNBinaryView* view, size_t* count);
	BINARYNINJACOREAPI BNSymbol** BNGetSymbolsInRange(BNBinaryView* view, uint64_t start, uint64_t len, size_t* count);
	BINARYNINJACOREAPI BNSymbol** BNGetSymbolsOfType(BNBinaryView* view, BNSymbolType type, size_t* count);
	BINARYNINJACOREAPI BNSymbol** BNGetSymbolsOfTypeInRange(BNBinaryView* view, BNSymbolType type,
	                                                        uint64_t start, uint64_t len, size_t* count);
	BINARYNINJACOREAPI void BNFreeSymbolList(BNSymbol** syms, size_t count);

	BINARYNINJACOREAPI void BNDefineAutoSymbol(BNBinaryView* view, BNSymbol* sym);
	BINARYNINJACOREAPI void BNUndefineAutoSymbol(BNBinaryView* view, BNSymbol* sym);
	BINARYNINJACOREAPI void BNDefineUserSymbol(BNBinaryView* view, BNSymbol* sym);
	BINARYNINJACOREAPI void BNUndefineUserSymbol(BNBinaryView* view, BNSymbol* sym);
	BINARYNINJACOREAPI void BNDefineImportedFunction(BNBinaryView* view, BNSymbol* importAddressSym, BNFunction* func);

	BINARYNINJACOREAPI BNSymbol* BNImportedFunctionFromImportAddressSymbol(BNSymbol* sym, uint64_t addr);

	// Low-level IL
	BINARYNINJACOREAPI BNLowLevelILFunction* BNCreateLowLevelILFunction(BNArchitecture* arch);
	BINARYNINJACOREAPI BNLowLevelILFunction* BNNewLowLevelILFunctionReference(BNLowLevelILFunction* func);
	BINARYNINJACOREAPI void BNFreeLowLevelILFunction(BNLowLevelILFunction* func);
	BINARYNINJACOREAPI uint64_t BNLowLevelILGetCurrentAddress(BNLowLevelILFunction* func);
	BINARYNINJACOREAPI void BNLowLevelILSetCurrentAddress(BNLowLevelILFunction* func, uint64_t addr);
	BINARYNINJACOREAPI void BNLowLevelILClearIndirectBranches(BNLowLevelILFunction* func);
	BINARYNINJACOREAPI void BNLowLevelILSetIndirectBranches(BNLowLevelILFunction* func, BNArchitectureAndAddress* branches,
	                                                        size_t count);
	BINARYNINJACOREAPI size_t BNLowLevelILAddExpr(BNLowLevelILFunction* func, BNLowLevelILOperation operation, size_t size,
	                                              uint32_t flags, uint64_t a, uint64_t b, uint64_t c, uint64_t d);
	BINARYNINJACOREAPI void BNLowLevelILSetExprSourceOperand(BNLowLevelILFunction* func, size_t expr, uint32_t operand);
	BINARYNINJACOREAPI size_t BNLowLevelILAddInstruction(BNLowLevelILFunction* func, size_t expr);
	BINARYNINJACOREAPI size_t BNLowLevelILGoto(BNLowLevelILFunction* func, BNLowLevelILLabel* label);
	BINARYNINJACOREAPI size_t BNLowLevelILIf(BNLowLevelILFunction* func, uint64_t op, BNLowLevelILLabel* t, BNLowLevelILLabel* f);
	BINARYNINJACOREAPI void BNLowLevelILInitLabel(BNLowLevelILLabel* label);
	BINARYNINJACOREAPI void BNLowLevelILMarkLabel(BNLowLevelILFunction* func, BNLowLevelILLabel* label);
	BINARYNINJACOREAPI void BNFinalizeLowLevelILFunction(BNLowLevelILFunction* func, BNFunction* sourceFunc);

	BINARYNINJACOREAPI size_t BNLowLevelILAddLabelList(BNLowLevelILFunction* func, BNLowLevelILLabel** labels, size_t count);
	BINARYNINJACOREAPI size_t BNLowLevelILAddOperandList(BNLowLevelILFunction* func, uint64_t* operands, size_t count);
	BINARYNINJACOREAPI uint64_t* BNLowLevelILGetOperandList(BNLowLevelILFunction* func, size_t expr, size_t operand,
	                                                        size_t* count);
	BINARYNINJACOREAPI void BNLowLevelILFreeOperandList(uint64_t* operands);

	BINARYNINJACOREAPI BNLowLevelILInstruction BNGetLowLevelILByIndex(BNLowLevelILFunction* func, size_t i);
	BINARYNINJACOREAPI size_t BNGetLowLevelILIndexForInstruction(BNLowLevelILFunction* func, size_t i);
	BINARYNINJACOREAPI size_t BNGetLowLevelILInstructionCount(BNLowLevelILFunction* func);

	BINARYNINJACOREAPI void BNAddLowLevelILLabelForAddress(BNLowLevelILFunction* func, BNArchitecture* arch, uint64_t addr);
	BINARYNINJACOREAPI BNLowLevelILLabel* BNGetLowLevelILLabelForAddress(BNLowLevelILFunction* func,
	                                                                     BNArchitecture* arch, uint64_t addr);

	BINARYNINJACOREAPI bool BNGetLowLevelILExprText(BNLowLevelILFunction* func, BNArchitecture* arch, size_t i,
		BNInstructionTextToken** tokens, size_t* count);
	BINARYNINJACOREAPI bool BNGetLowLevelILInstructionText(BNLowLevelILFunction* il, BNFunction* func,
		BNArchitecture* arch, size_t i, BNInstructionTextToken** tokens, size_t* count);

	BINARYNINJACOREAPI uint32_t BNGetLowLevelILTemporaryRegisterCount(BNLowLevelILFunction* func);
	BINARYNINJACOREAPI uint32_t BNGetLowLevelILTemporaryFlagCount(BNLowLevelILFunction* func);

	BINARYNINJACOREAPI BNBasicBlock** BNGetLowLevelILBasicBlockList(BNLowLevelILFunction* func, size_t* count);

	// Types
	BINARYNINJACOREAPI BNType* BNCreateVoidType(void);
	BINARYNINJACOREAPI BNType* BNCreateBoolType(void);
	BINARYNINJACOREAPI BNType* BNCreateIntegerType(size_t width, bool sign, const char* altName);
	BINARYNINJACOREAPI BNType* BNCreateFloatType(size_t width, const char* altName);
	BINARYNINJACOREAPI BNType* BNCreateStructureType(BNStructure* s);
	BINARYNINJACOREAPI BNType* BNCreateEnumerationType(BNArchitecture* arch, BNEnumeration* e, size_t width, bool isSigned);
	BINARYNINJACOREAPI BNType* BNCreatePointerType(BNArchitecture* arch, BNType* type, bool cnst, bool vltl,
	                                               BNReferenceType refType);
	BINARYNINJACOREAPI BNType* BNCreateArrayType(BNType* type, uint64_t elem);
	BINARYNINJACOREAPI BNType* BNCreateFunctionType(BNType* returnValue, BNCallingConvention* callingConvention,
	                                                BNNameAndType* params, size_t paramCount, bool varArg);
	BINARYNINJACOREAPI BNType* BNNewTypeReference(BNType* type);
	BINARYNINJACOREAPI BNType* BNDuplicateType(BNType* type);
	BINARYNINJACOREAPI char* BNGetTypeAndName(BNType* type, const char** nameList, size_t nameCount);
	BINARYNINJACOREAPI void BNFreeType(BNType* type);

	BINARYNINJACOREAPI BNTypeClass BNGetTypeClass(BNType* type);
	BINARYNINJACOREAPI uint64_t BNGetTypeWidth(BNType* type);
	BINARYNINJACOREAPI size_t BNGetTypeAlignment(BNType* type);
	BINARYNINJACOREAPI bool BNIsTypeSigned(BNType* type);
	BINARYNINJACOREAPI bool BNIsTypeConst(BNType* type);
	BINARYNINJACOREAPI bool BNIsTypeVolatile(BNType* type);
	BINARYNINJACOREAPI bool BNIsTypeFloatingPoint(BNType* type);
	BINARYNINJACOREAPI BNType* BNGetChildType(BNType* type);
	BINARYNINJACOREAPI BNCallingConvention* BNGetTypeCallingConvention(BNType* type);
	BINARYNINJACOREAPI BNNameAndType* BNGetTypeParameters(BNType* type, size_t* count);
	BINARYNINJACOREAPI void BNFreeTypeParameterList(BNNameAndType* types, size_t count);
	BINARYNINJACOREAPI bool BNTypeHasVariableArguments(BNType* type);
	BINARYNINJACOREAPI bool BNFunctionTypeCanReturn(BNType* type);
	BINARYNINJACOREAPI BNStructure* BNGetTypeStructure(BNType* type);
	BINARYNINJACOREAPI BNEnumeration* BNGetTypeEnumeration(BNType* type);
	BINARYNINJACOREAPI uint64_t BNGetTypeElementCount(BNType* type);
	BINARYNINJACOREAPI void BNSetFunctionCanReturn(BNType* type, bool canReturn);

	BINARYNINJACOREAPI char* BNGetTypeString(BNType* type);
	BINARYNINJACOREAPI char* BNGetTypeStringBeforeName(BNType* type);
	BINARYNINJACOREAPI char* BNGetTypeStringAfterName(BNType* type);

	BINARYNINJACOREAPI BNStructure* BNCreateStructure(void);
	BINARYNINJACOREAPI BNStructure* BNNewStructureReference(BNStructure* s);
	BINARYNINJACOREAPI void BNFreeStructure(BNStructure* s);

	BINARYNINJACOREAPI char** BNGetStructureName(BNStructure* s, size_t* size);
	BINARYNINJACOREAPI void BNSetStructureName(BNStructure* s, const char** names, size_t size);
	BINARYNINJACOREAPI BNStructureMember* BNGetStructureMembers(BNStructure* s, size_t* count);
	BINARYNINJACOREAPI void BNFreeStructureMemberList(BNStructureMember* members, size_t count);
	BINARYNINJACOREAPI uint64_t BNGetStructureWidth(BNStructure* s);
	BINARYNINJACOREAPI size_t BNGetStructureAlignment(BNStructure* s);
	BINARYNINJACOREAPI bool BNIsStructurePacked(BNStructure* s);
	BINARYNINJACOREAPI void BNSetStructurePacked(BNStructure* s, bool packed);
	BINARYNINJACOREAPI bool BNIsStructureUnion(BNStructure* s);
	BINARYNINJACOREAPI void BNSetStructureUnion(BNStructure* s, bool u);

	BINARYNINJACOREAPI void BNAddStructureMember(BNStructure* s, BNType* type, const char* name);
	BINARYNINJACOREAPI void BNAddStructureMemberAtOffset(BNStructure* s, BNType* type, const char* name, uint64_t offset);
	BINARYNINJACOREAPI void BNRemoveStructureMember(BNStructure* s, size_t idx);

	BINARYNINJACOREAPI BNEnumeration* BNCreateEnumeration(void);
	BINARYNINJACOREAPI BNEnumeration* BNNewEnumerationReference(BNEnumeration* e);
	BINARYNINJACOREAPI void BNFreeEnumeration(BNEnumeration* e);

	BINARYNINJACOREAPI char** BNGetEnumerationName(BNEnumeration* e, size_t* size);
	BINARYNINJACOREAPI void BNSetEnumerationName(BNEnumeration* e, const char** name, size_t size);
	BINARYNINJACOREAPI BNEnumerationMember* BNGetEnumerationMembers(BNEnumeration* e, size_t* count);
	BINARYNINJACOREAPI void BNFreeEnumerationMemberList(BNEnumerationMember* members, size_t count);

	BINARYNINJACOREAPI void BNAddEnumerationMember(BNEnumeration* e, const char* name);
	BINARYNINJACOREAPI void BNAddEnumerationMemberWithValue(BNEnumeration* e, const char* name, uint64_t value);

	// Source code processing
	BINARYNINJACOREAPI bool BNPreprocessSource(const char* source, const char* fileName, char** output, char** errors,
	                                           const char** includeDirs, size_t includeDirCount);
	BINARYNINJACOREAPI bool BNParseTypesFromSource(BNArchitecture* arch, const char* source, const char* fileName,
	                                               BNTypeParserResult* result, char** errors,
	                                               const char** includeDirs, size_t includeDirCount);
	BINARYNINJACOREAPI bool BNParseTypesFromSourceFile(BNArchitecture* arch, const char* fileName,
	                                                   BNTypeParserResult* result, char** errors,
	                                                   const char** includeDirs, size_t includeDirCount);
	BINARYNINJACOREAPI void BNFreeTypeParserResult(BNTypeParserResult* result);

	// Updates
	BINARYNINJACOREAPI BNUpdateChannel* BNGetUpdateChannels(size_t* count, char** errors);
	BINARYNINJACOREAPI void BNFreeUpdateChannelList(BNUpdateChannel* list, size_t count);
	BINARYNINJACOREAPI BNUpdateVersion* BNGetUpdateChannelVersions(const char* channel, size_t* count, char** errors);
	BINARYNINJACOREAPI void BNFreeUpdateChannelVersionList(BNUpdateVersion* list, size_t count);

	BINARYNINJACOREAPI bool BNAreUpdatesAvailable(const char* channel, char** errors);

	BINARYNINJACOREAPI BNUpdateResult BNUpdateToVersion(const char* channel, const char* version, char** errors,
	                                                    bool (*progress)(void* ctxt, uint64_t progress, uint64_t total),
	                                                    void* context);
	BINARYNINJACOREAPI BNUpdateResult BNUpdateToLatestVersion(const char* channel, char** errors,
	                                                          bool (*progress)(void* ctxt, uint64_t progress, uint64_t total),
	                                                          void* context);

	BINARYNINJACOREAPI bool BNAreAutoUpdatesEnabled(void);
	BINARYNINJACOREAPI void BNSetAutoUpdatesEnabled(bool enabled);
	BINARYNINJACOREAPI uint64_t BNGetTimeSinceLastUpdateCheck(void);
	BINARYNINJACOREAPI void BNUpdatesChecked(void);

	BINARYNINJACOREAPI char* BNGetActiveUpdateChannel(void);
	BINARYNINJACOREAPI void BNSetActiveUpdateChannel(const char* channel);

	BINARYNINJACOREAPI bool BNIsUpdateInstallationPending(void);
	BINARYNINJACOREAPI void BNInstallPendingUpdate(char** errors);

	// Plugin commands
	BINARYNINJACOREAPI void BNRegisterPluginCommand(const char* name, const char* description,
	                                                void (*action)(void* ctxt, BNBinaryView* view),
	                                                bool (*isValid)(void* ctxt, BNBinaryView* view), void* context);
	BINARYNINJACOREAPI void BNRegisterPluginCommandForAddress(const char* name, const char* description,
	                                                          void (*action)(void* ctxt, BNBinaryView* view, uint64_t addr),
	                                                          bool (*isValid)(void* ctxt, BNBinaryView* view, uint64_t addr),
	                                                          void* context);
	BINARYNINJACOREAPI void BNRegisterPluginCommandForRange(const char* name, const char* description,
	                                                        void (*action)(void* ctxt, BNBinaryView* view, uint64_t addr, uint64_t len),
	                                                        bool (*isValid)(void* ctxt, BNBinaryView* view, uint64_t addr, uint64_t len),
	                                                        void* context);
	BINARYNINJACOREAPI void BNRegisterPluginCommandForFunction(const char* name, const char* description,
	                                                           void (*action)(void* ctxt, BNBinaryView* view, BNFunction* func),
	                                                           bool (*isValid)(void* ctxt, BNBinaryView* view, BNFunction* func),
	                                                           void* context);

	BINARYNINJACOREAPI BNPluginCommand* BNGetAllPluginCommands(size_t* count);
	BINARYNINJACOREAPI BNPluginCommand* BNGetValidPluginCommands(BNBinaryView* view, size_t* count);
	BINARYNINJACOREAPI BNPluginCommand* BNGetValidPluginCommandsForAddress(BNBinaryView* view, uint64_t addr,
	                                                                       size_t* count);
	BINARYNINJACOREAPI BNPluginCommand* BNGetValidPluginCommandsForRange(BNBinaryView* view, uint64_t addr,
	                                                                     uint64_t len, size_t* count);
	BINARYNINJACOREAPI BNPluginCommand* BNGetValidPluginCommandsForFunction(BNBinaryView* view, BNFunction* func,
	                                                                        size_t* count);
	BINARYNINJACOREAPI void BNFreePluginCommandList(BNPluginCommand* commands);

	// Calling conventions
	BINARYNINJACOREAPI BNCallingConvention* BNCreateCallingConvention(BNArchitecture* arch, const char* name,
	                                                                  BNCustomCallingConvention* cc);
	BINARYNINJACOREAPI void BNRegisterCallingConvention(BNArchitecture* arch, BNCallingConvention* cc);
	BINARYNINJACOREAPI BNCallingConvention* BNNewCallingConventionReference(BNCallingConvention* cc);
	BINARYNINJACOREAPI void BNFreeCallingConvention(BNCallingConvention* cc);

	BINARYNINJACOREAPI BNCallingConvention** BNGetArchitectureCallingConventions(BNArchitecture* arch, size_t* count);
	BINARYNINJACOREAPI void BNFreeCallingConventionList(BNCallingConvention** list, size_t count);
	BINARYNINJACOREAPI BNCallingConvention* BNGetArchitectureCallingConventionByName(BNArchitecture* arch,
	                                                                                 const char* name);

	BINARYNINJACOREAPI BNArchitecture* BNGetCallingConventionArchitecture(BNCallingConvention* cc);
	BINARYNINJACOREAPI char* BNGetCallingConventionName(BNCallingConvention* cc);
	BINARYNINJACOREAPI uint32_t* BNGetCallerSavedRegisters(BNCallingConvention* cc, size_t* count);

	BINARYNINJACOREAPI uint32_t* BNGetIntegerArgumentRegisters(BNCallingConvention* cc, size_t* count);
	BINARYNINJACOREAPI uint32_t* BNGetFloatArgumentRegisters(BNCallingConvention* cc, size_t* count);
	BINARYNINJACOREAPI bool BNAreArgumentRegistersSharedIndex(BNCallingConvention* cc);
	BINARYNINJACOREAPI bool BNIsStackReservedForArgumentRegisters(BNCallingConvention* cc);

	BINARYNINJACOREAPI uint32_t BNGetIntegerReturnValueRegister(BNCallingConvention* cc);
	BINARYNINJACOREAPI uint32_t BNGetHighIntegerReturnValueRegister(BNCallingConvention* cc);
	BINARYNINJACOREAPI uint32_t BNGetFloatReturnValueRegister(BNCallingConvention* cc);

	BINARYNINJACOREAPI BNCallingConvention* BNGetArchitectureDefaultCallingConvention(BNArchitecture* arch);
	BINARYNINJACOREAPI BNCallingConvention* BNGetArchitectureCdeclCallingConvention(BNArchitecture* arch);
	BINARYNINJACOREAPI BNCallingConvention* BNGetArchitectureStdcallCallingConvention(BNArchitecture* arch);
	BINARYNINJACOREAPI BNCallingConvention* BNGetArchitectureFastcallCallingConvention(BNArchitecture* arch);
	BINARYNINJACOREAPI void BNSetArchitectureDefaultCallingConvention(BNArchitecture* arch, BNCallingConvention* cc);
	BINARYNINJACOREAPI void BNSetArchitectureCdeclCallingConvention(BNArchitecture* arch, BNCallingConvention* cc);
	BINARYNINJACOREAPI void BNSetArchitectureStdcallCallingConvention(BNArchitecture* arch, BNCallingConvention* cc);
	BINARYNINJACOREAPI void BNSetArchitectureFastcallCallingConvention(BNArchitecture* arch, BNCallingConvention* cc);

	// Platforms
	BINARYNINJACOREAPI BNPlatform* BNCreatePlatform(BNArchitecture* arch, const char* name);
	BINARYNINJACOREAPI void BNRegisterPlatform(const char* os, BNPlatform* platform);
	BINARYNINJACOREAPI BNPlatform* BNNewPlatformReference(BNPlatform* platform);
	BINARYNINJACOREAPI void BNFreePlatform(BNPlatform* platform);

	BINARYNINJACOREAPI char* BNGetPlatformName(BNPlatform* platform);
	BINARYNINJACOREAPI BNArchitecture* BNGetPlatformArchitecture(BNPlatform* platform);

	BINARYNINJACOREAPI BNPlatform* BNGetPlatformByName(const char* name);
	BINARYNINJACOREAPI BNPlatform** BNGetPlatformList(size_t* count);
	BINARYNINJACOREAPI BNPlatform** BNGetPlatformListByArchitecture(BNArchitecture* arch, size_t* count);
	BINARYNINJACOREAPI BNPlatform** BNGetPlatformListByOS(const char* os, size_t* count);
	BINARYNINJACOREAPI BNPlatform** BNGetPlatformListByOSAndArchitecture(const char* os, BNArchitecture* arch,
	                                                                     size_t* count);
	BINARYNINJACOREAPI void BNFreePlatformList(BNPlatform** platform, size_t count);
	BINARYNINJACOREAPI char** BNGetPlatformOSList(size_t* count);
	BINARYNINJACOREAPI void BNFreePlatformOSList(char** list, size_t count);

	BINARYNINJACOREAPI BNCallingConvention* BNGetPlatformDefaultCallingConvention(BNPlatform* platform);
	BINARYNINJACOREAPI BNCallingConvention* BNGetPlatformCdeclCallingConvention(BNPlatform* platform);
	BINARYNINJACOREAPI BNCallingConvention* BNGetPlatformStdcallCallingConvention(BNPlatform* platform);
	BINARYNINJACOREAPI BNCallingConvention* BNGetPlatformFastcallCallingConvention(BNPlatform* platform);
	BINARYNINJACOREAPI BNCallingConvention** BNGetPlatformCallingConventions(BNPlatform* platform, size_t* count);
	BINARYNINJACOREAPI BNCallingConvention* BNGetPlatformSystemCallConvention(BNPlatform* platform);

	BINARYNINJACOREAPI void BNRegisterPlatformCallingConvention(BNPlatform* platform, BNCallingConvention* cc);
	BINARYNINJACOREAPI void BNRegisterPlatformDefaultCallingConvention(BNPlatform* platform, BNCallingConvention* cc);
	BINARYNINJACOREAPI void BNRegisterPlatformCdeclCallingConvention(BNPlatform* platform, BNCallingConvention* cc);
	BINARYNINJACOREAPI void BNRegisterPlatformStdcallCallingConvention(BNPlatform* platform, BNCallingConvention* cc);
	BINARYNINJACOREAPI void BNRegisterPlatformFastcallCallingConvention(BNPlatform* platform, BNCallingConvention* cc);
	BINARYNINJACOREAPI void BNSetPlatformSystemCallConvention(BNPlatform* platform, BNCallingConvention* cc);

	BINARYNINJACOREAPI BNPlatform* BNGetArchitectureStandalonePlatform(BNArchitecture* arch);

	BINARYNINJACOREAPI BNPlatform* BNGetRelatedPlatform(BNPlatform* platform, BNArchitecture* arch);
	BINARYNINJACOREAPI void BNAddRelatedPlatform(BNPlatform* platform, BNArchitecture* arch, BNPlatform* related);

	//Demangler
	BINARYNINJACOREAPI bool BNDemangleMS(BNArchitecture* arch,
	                                     const char* mangledName,
	                                     BNType** outType,
	                                     char*** outVarName,
	                                     size_t* outVarNameElements);

	// Scripting providers
	BINARYNINJACOREAPI BNScriptingProvider* BNRegisterScriptingProvider(const char* name,
		BNScriptingProviderCallbacks* callbacks);
	BINARYNINJACOREAPI BNScriptingProvider** BNGetScriptingProviderList(size_t* count);
	BINARYNINJACOREAPI void BNFreeScriptingProviderList(BNScriptingProvider** providers);
	BINARYNINJACOREAPI BNScriptingProvider* BNGetScriptingProviderByName(const char* name);

	BINARYNINJACOREAPI char* BNGetScriptingProviderName(BNScriptingProvider* provider);
	BINARYNINJACOREAPI BNScriptingInstance* BNCreateScriptingProviderInstance(BNScriptingProvider* provider);

	BINARYNINJACOREAPI BNScriptingInstance* BNInitScriptingInstance(BNScriptingProvider* provider,
		BNScriptingInstanceCallbacks* callbacks);
	BINARYNINJACOREAPI BNScriptingInstance* BNNewScriptingInstanceReference(BNScriptingInstance* instance);
	BINARYNINJACOREAPI void BNFreeScriptingInstance(BNScriptingInstance* instance);
	BINARYNINJACOREAPI void BNNotifyOutputForScriptingInstance(BNScriptingInstance* instance, const char* text);
	BINARYNINJACOREAPI void BNNotifyErrorForScriptingInstance(BNScriptingInstance* instance, const char* text);
	BINARYNINJACOREAPI void BNNotifyInputReadyStateForScriptingInstance(BNScriptingInstance* instance,
		BNScriptingProviderInputReadyState state);

	BINARYNINJACOREAPI void BNRegisterScriptingInstanceOutputListener(BNScriptingInstance* instance,
		BNScriptingOutputListener* callbacks);
	BINARYNINJACOREAPI void BNUnregisterScriptingInstanceOutputListener(BNScriptingInstance* instance,
		BNScriptingOutputListener* callbacks);

	BINARYNINJACOREAPI BNScriptingProviderInputReadyState BNGetScriptingInstanceInputReadyState(
		BNScriptingInstance* instance);
	BINARYNINJACOREAPI BNScriptingProviderExecuteResult BNExecuteScriptInput(BNScriptingInstance* instance,
		const char* input);
	BINARYNINJACOREAPI void BNSetScriptingInstanceCurrentBinaryView(BNScriptingInstance* instance, BNBinaryView* view);
	BINARYNINJACOREAPI void BNSetScriptingInstanceCurrentFunction(BNScriptingInstance* instance, BNFunction* func);
	BINARYNINJACOREAPI void BNSetScriptingInstanceCurrentBasicBlock(BNScriptingInstance* instance, BNBasicBlock* block);
	BINARYNINJACOREAPI void BNSetScriptingInstanceCurrentAddress(BNScriptingInstance* instance, uint64_t addr);
	BINARYNINJACOREAPI void BNSetScriptingInstanceCurrentSelection(BNScriptingInstance* instance,
		uint64_t begin, uint64_t end);

	// Main thread actions
	BINARYNINJACOREAPI void BNRegisterMainThread(BNMainThreadCallbacks* callbacks);
	BINARYNINJACOREAPI BNMainThreadAction* BNNewMainThreadActionReference(BNMainThreadAction* action);
	BINARYNINJACOREAPI void BNFreeMainThreadAction(BNMainThreadAction* action);
	BINARYNINJACOREAPI void BNExecuteMainThreadAction(BNMainThreadAction* action);
	BINARYNINJACOREAPI bool BNIsMainThreadActionDone(BNMainThreadAction* action);
	BINARYNINJACOREAPI void BNWaitForMainThreadAction(BNMainThreadAction* action);
	BINARYNINJACOREAPI BNMainThreadAction* BNExecuteOnMainThread(void* ctxt, void (*func)(void* ctxt));
	BINARYNINJACOREAPI void BNExecuteOnMainThreadAndWait(void* ctxt, void (*func)(void* ctxt));

#ifdef __cplusplus
}
#endif

#endif
