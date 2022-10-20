/**
 * Copyright L. Spiro 2021
 *
 * Written by: Shawn (L. Spiro) Wilcoxen
 *
 * Description: Enough emulation of a Ricoh 6502 CPU to run a Nintendo Entertainment System.
 * 
 * https://www.nesdev.org/6502_cpu.txt
 * http://www.oxyron.de/html/opcodes02.html
 * http://www.6502.org/tutorials/6502opcodes.html
 * http://users.telenet.be/kim1-6502/6502/proman.html
 * http://problemkaputt.de/everynes.htm#cpu65xxmicroprocessor
 * https://www.masswerk.at/6502/6502_instruction_set.html
 */


#pragma once

#include "../LSNLSpiroNes.h"
#include "../Bus/LSNBus.h"
#include "../System/LSNTickable.h"
#include <vector>

namespace lsn {

	/**
	 * Class CCpu6502
	 * \brief Enough emulation of a Ricoh 6502 CPU to run a Nintendo Entertainment System.
	 *
	 * Description: Enough emulation of a Ricoh 6502 CPU to run a Nintendo Entertainment System.
	 */
	class CCpu6502 : public CTickable {
	public :
		// == Various constructors.
		CCpu6502( CCpuBus * _pbBus );
		~CCpu6502();


		// == Enumerations.
		/** Status flags. */
		enum class LSN_STATUS_FLAGS : uint8_t {
			LSN_SF_CARRY					= 1 << 0,										/**< Carry         (0=No Carry, 1=Carry) */
			LSN_SF_ZERO						= 1 << 1,										/**< Zero          (0=Nonzero, 1=Zero) */
			LSN_SF_IRQ						= 1 << 2,										/**< IRQ Disable   (0=IRQ Enable, 1=IRQ Disable) */
			LSN_SF_DECIMAL					= 1 << 3,										/**< Decimal Mode  (0=Normal, 1=BCD Mode for ADC/SBC opcodes) */
			LSN_SF_BREAK					= 1 << 4,										/**< Break Flag    (0=IRQ/NMI, 1=RESET or BRK/PHP opcode) */
			LSN_SF_RESERVED					= 1 << 5,										/**< Reserved. */
			LSN_SF_OVERFLOW					= 1 << 6,										/**< Overflow      (0=No Overflow, 1=Overflow) */
			LSN_SF_NEGATIVE					= 1 << 7,										/**< Negative/Sign (0=Positive, 1=Negative) */
		};

		/** Special addresses. */
		enum class LSN_VECTORS : uint16_t {
			LSN_V_NMI						= 0xFFFA,										/**< The address of execution during an NMI interrupt. */
			LSN_V_RESET						= 0xFFFC,										/**< The address of execution during a reset. */
			LSN_V_IRQ_BRK					= 0xFFFE,										/**< The address of execution during an IRQ or BRK interrupt. */
		};

		/** Special opcodes. */
		enum LSN_SPECIAL_OPS {
			LSN_SO_NMI						= 0x100,										/**< The NMI instruction. */
			LSN_SO_IRQ						= 0x101,										/**< The NMI instruction. */
		};


		// == Functions.
		/**
		 * Resets the CPU to a known state.
		 */
		void								ResetToKnown();

		/**
		 * Performs an "analog" reset, allowing previous data to remain.
		 */
		void								ResetAnalog();

		/**
		 * Performs a single cycle update.
		 */
		virtual void						Tick();

		/**
		 * DESC
		 *
		 * \param PARM DESC
		 * \param PARM DESC
		 * \return RETURN
		 */
		//void								Irq();

		/**
		 * DESC
		 *
		 * \param PARM DESC
		 * \param PARM DESC
		 * \return RETURN
		 */
		//void								Nmi();


	protected :
		// == Enumerations.
		/** Addressing modes. */
		enum LSN_ADDRESSING_MODES : uint8_t {
			LSN_AM_IMPLIED,																	/**< Implied addressing = 1 extra cycle. */
			LSN_AM_ACCUMULATOR,																/**< Accumulator = 1 extra cycle. */
			LSN_AM_IMMEDIATE,																/**< Immediate = 1 extra cycle. */
			LSN_AM_ZERO_PAGE,																/**< Zero-page = 2 extra cycles. */
			LSN_AM_ZERO_PAGE_X,																/**< Zero-page-X = 3 extra cycles. */
			LSN_AM_ZERO_PAGE_Y,																/**< Zero-page-Y = 3 extra cycles. */
			LSN_AM_RELATIVE,																/**< Relative jumping = 2 extra cycles. */
			LSN_AM_ABSOLUTE,																/**< Absolute addressing = 3 extra cycles. */
			LSN_AM_ABSOLUTE_X,																/**< Absolute X addressing = 3 extra cycles + optional cycle. */
			LSN_AM_ABSOLUTE_Y,																/**< Absolute Y addressing = 3 extra cycles + optional cycle. */
			LSN_AM_INDIRECT_X,																/**< Indirect X addressing = 5 extra cycles. */
			LSN_AM_INDIRECT_Y,																/**< Indirect Y addressing = 4 extra cycles + optional cycle. */
			LSN_AM_INDIRECT,																/**< Absolute indirect addressing (JMP) = 4 extra cycles. */
		};
		/** Instructions. Numbers specified because these are used in look-up tables. */
		enum LSN_INSTRUCTIONS : uint8_t {
			LSN_I_ADC						= 0,											/**< Add with carry. */
			LSN_I_AND						= 1,											/**< AND with accumulator. */
			LSN_I_ASL						= 2,											/**< Arithmetic shift left. */
			LSN_I_BCC						= 3,											/**< Branch on carry clear. */
			LSN_I_BCS						= 4,											/**< Branch on carry set. */
			LSN_I_BEQ						= 5,											/**< Branch on equal (zero set). */
			LSN_I_BIT						= 6,											/**< Bit test. */
			LSN_I_BMI						= 7,											/**< Branch on minus (negative set). */
			LSN_I_BNE						= 8,											/**< Branch on not equal (zero clear). */
			LSN_I_BPL						= 9,											/**< Branch on plus (negative clear). */
			LSN_I_BRK						= 10,											/**< Break/interrupt. */
			LSN_I_BVC						= 11,											/**< Branch on overflow clear. */
			LSN_I_BVS						= 12,											/**< Branch on overflow set. */
			LSN_I_CLC						= 13,											/**< Clear carry. */
			LSN_I_CLD						= 14,											/**< Clear decimal. */
			LSN_I_CLI						= 15,											/**< Clear interrupt-disable. */
			LSN_I_CLV						= 16,											/**< Clear overflow. */
			LSN_I_CMP						= 17,											/**< Compare (with accumulator). */
			LSN_I_CPX						= 18,											/**< Compare with X. */
			LSN_I_CPY						= 19,											/**< Compare with Y. */
			LSN_I_DEC						= 20,											/**< Decrement. */
			LSN_I_DEX						= 21,											/**< Decrement X. */
			LSN_I_DEY						= 22,											/**< Decrement Y. */
			LSN_I_EOR						= 23,											/**< XOR with accumulator. */
			LSN_I_INC						= 24,											/**< Increment. */
			LSN_I_INX						= 25,											/**< Increment X. */
			LSN_I_INY						= 26,											/**< Increment Y. */
			LSN_I_JMP						= 27,											/**< Jump. */
			LSN_I_JSR						= 28,											/**< Jump subroutine. */
			LSN_I_LDA						= 29,											/**< Load accumulator. */
			LSN_I_LDX						= 30,											/**< Load X. */
			LSN_I_LDY						= 31,											/**< Load Y. */
			LSN_I_LSR						= 32,											/**< Logical shift right. */
			LSN_I_NOP						= 33,											/**< No operation. */
			LSN_I_ORA						= 34,											/**< OR with accumulator. */
			LSN_I_PHA						= 35,											/**< Push accumulator. */
			LSN_I_PHP						= 36,											/**< Push processor status (SR). */
			LSN_I_PLA						= 37,											/**< Pull accumulator. */
			LSN_I_PLP						= 38,											/**< Pull processor status (SR). */
			LSN_I_ROL						= 39,											/**< Rotate left. */
			LSN_I_ROR						= 40,											/**< Rotate right. */
			LSN_I_RTI						= 41,											/**< Return from interrupt. */
			LSN_I_RTS						= 42,											/**< Return from subroutine. */
			LSN_I_SBC						= 43,											/**< Subtract with carry. */
			LSN_I_SEC						= 44,											/**< Set carry. */
			LSN_I_SED						= 45,											/**< Set decimal. */
			LSN_I_SEI						= 46,											/**< Set interrupt-disable. */
			LSN_I_STA						= 47,											/**< Store accumulator. */
			LSN_I_STX						= 48,											/**< Store X. */
			LSN_I_STY						= 49,											/**< Store Y. */
			LSN_I_TAX						= 50,											/**< Transfer accumulator to X. */
			LSN_I_TAY						= 51,											/**< Transfer accumulator to Y. */
			LSN_I_TSX						= 52,											/**< Transfer stack pointer to X. */
			LSN_I_TXA						= 53,											/**< Transfer X to accumulator. */
			LSN_I_TXS						= 54,											/**< Transfer X to stack pointer. */
			LSN_I_TYA						= 55,											/**< Transfer Y to accumulator. */

			LSN_I_ALR						= 56,											/**< Illegal. Performs A & OP; LSR. */
			LSN_I_ASR						= 56,											/**< Same as LSN_I_ALR. */
			LSN_I_ANC						= 57,											/**< Illegal. Bitwise AND with carry. C flag is set using ASL rules. */
			LSN_I_ANC2						= 58,											/**< Illegal. Bitwise AND with carry. C flag is set using ROL rules. */
			LSN_I_ANE						= 59,											/**< Illegal and highly unstable. Performs A = (A | CONST) & X & OP, where CONST is effectively random. */
			LSN_I_ARR						= 60,											/**< Illegal. Performs A = A & OP; A = (A >> 1) | (C << 7). */
			LSN_I_DCP						= 61,											/**< Illegal. Performs OP--; CMP(A, OP). */
			LSN_I_ISC						= 62,											/**< Illegal. Performs INC; SBC. */
			LSN_I_ISB						= 62,											/**< Same as LSN_I_ISC. */
			LSN_I_LAS						= 63,											/**< Illegal. Performs A = X = S = (OP & S). */
			LSN_I_LAX						= 64,											/**< Illegal. Performs A = X = OP. */
			LSN_I_LXA						= 65,											/**< Illegal and highly unstable. Performs X = A = ((A | CONST) & OP), where CONST is effectively random. */
			LSN_I_RLA						= 66,											/**< Illegal. ROL OP, A &= OP. */
			LSN_I_RRA						= 67,											/**< Illegal. ROR OP, A += OP + C. */
			LSN_I_SAX						= 68,											/**< Illegal. Performs OP = A & X. */
			LSN_I_SBX						= 69,											/**< Illegal. Performs X = (A & X) - OP, setting flags as with CMP. */
			LSN_I_SHA						= 70,											/**< Illegal. Stores A & X & (high-byte of address + 1) at the address. */
			LSN_I_SHX						= 71,											/**< Illegal. Stores X & (high-byte of address + 1) at the address. */
			LSN_I_SHY						= 72,											/**< Illegal. Stores Y & (high-byte of address + 1) at the address. */
			LSN_I_SLO						= 73,											/**< Illegal. OP <<= 1, A &= OP. */
			LSN_I_SRE						= 74,											/**< Illegal. OP >>= 1, A ^= OP. */
			LSN_I_TAS						= 75,											/**< Illegal. Performs S = A & X; Stores A & X & (high-byte of address + 1) at the address. */
			LSN_I_SHS						= 75,											/**< Same as LSN_I_TAS. */
			LSN_I_USBC						= 76,											/**< Illegal. Performs SBC + NOP. */
			LSN_I_DOP						= 77,											/**< No operation. */
			LSN_I_TOP						= 78,											/**< No operation. */
			LSN_I_JAM						= 79,											/**< Illegal. Jams the machine repeatedly putting 0xFF on the bus. */

			LSN_I_NMI						= 80,											/**< Non-opcode: NMI. */
			LSN_I_IRQ						= 81,											/**< Non-opcode: IRQ. */

			LSN_I_TOTAL
		};

		/** Other parameters. */
		enum LSN_MISC {
			LSN_M_MAX_INSTR_CYCLE_COUNT		= 7,
		};

		/** Contexts. */
		enum LSN_CONTEXTS : uint8_t {
			LSN_C_NORMAL																	/**< Normal processing.  Execute the current cycle of the current instruction or move on to the next instruction. */

		};

		
		// == Types.
		/** Data for the current context of execution (most often just regular instruction execution). */
		struct LSN_CPU_CONTEXT {
			uint16_t						ui16OpCode;										/**< The current opcode. */
			union {
				uint16_t					ui16Address;									/**< For various access types. */
				uint8_t						ui8Bytes[2];
			}								a;
			union {
				uint16_t					ui16JmpTarget;
				uint8_t						ui8Bytes[2];
			}								j;
			//LSN_CONTEXTS					cContext;										/**< The context for whatever is currently being processed. */
			uint8_t							ui8Cycle;										/**< The per-instruction cycle count. */
			uint8_t							ui8FuncIdx;										/**< The index of the next function to call in the instruction. */
			uint8_t							ui8Operand;										/**< The operand and low byte of addresses. */
			uint8_t							ui8Pointer;										/**< For indirect indexed access. */
		};

		/** A function pointer for the functions that handle each cycle. */
		typedef void (CCpu6502:: *			PfCycle)();

		/** An instruction. The micro-functions (pfHandler) that make up each cycle of each instruction are programmed to know what to do and can correctly pass the cycles without
		 *	using ui8TotalCycles or amAddrMode. This means pcName, ui8TotalCycles, and amAddrMode are only used for debugging, verification, printing things, etc.
		 * Since we are adding work by increasing the number of functions calls per instruction, we get that time back by not checking for addressing modes or referencing any other
		 *	tables or data.  For the sake of performance, each micro-function just knows what to do and does so in the most efficient manner possible, free from any unnecessary
		 *	branching etc.
		 * pfHandler points to an array of functions that can handle all possible cycles for a given instruction, and we use a subtractive process for eliminating optional cycles
		 *	rather than using the additive approach most commonly found in emulators.
		 */
		struct LSN_INSTR {
			PfCycle							pfHandler[LSN_M_MAX_INSTR_CYCLE_COUNT];			/**< Indexed by LSN_CPU_CONTEXT::ui8FuncIdx, these functions handle each cycle of the instruction. */
			uint8_t							ui8TotalCycles;									/**< Total non-optional non-overlapping cycles in the instruction. Used only for debugging, disassembling, etc. */
			LSN_ADDRESSING_MODES			amAddrMode;										/**< Addressing mode. Used only for debugging, disassembling, etc. */
			uint8_t							ui8Size;										/**< Size in bytes of the instruction. Used only for debugging, disassembling, etc. */
			LSN_INSTRUCTIONS				iInstruction;									/**< The instruction. */
		};

		/** Instruction data for assembly/disassembly. */
		struct LSN_INSTR_META_DATA {
			const char *					pcName;											/**< The name of the instruction. */
			const char8_t *					putf8Desc;										/**< The instruction description. */
		};


		// == Members.
		CCpuBus *							m_pbBus;										/**< Pointer to the bus. */
		LSN_CPU_CONTEXT *					m_pccCurContext;								/**< Always points to the top of the stack but it is set as sparsely as possible so as to avoid recalculatig it each cycle. */
		std::vector<LSN_CPU_CONTEXT>		m_vContextStack;								/**< Stack of contexts. */
		union {
			uint16_t						PC;												/**< Program counter. */
			uint8_t							ui8Bytes[2];
		}									pc;
		uint8_t								A;												/**< Accumulator. */
		uint8_t								X;												/**< Index register X. */
		uint8_t								Y;												/**< Index register Y. */
		uint8_t								S;												/**< Stack pointer (addresses 0x0100 + S). */
		uint8_t								m_ui8Status;									/**< The status flags. */
		bool								m_bNmiHistory[2];								/**< The NMI value for the last cycle and the current cycle. */
		bool								m_bHandleNmi;									/**< Once an NMI edge is detected, this is set to indicate that it needs to be handled. */
		
		static LSN_INSTR					m_iInstructionSet[256+2];						/**< The instruction set. */
		static const LSN_INSTR_META_DATA	m_smdInstMetaData[LSN_I_TOTAL];					/**< Metadata for the instructions (for assembly and disassembly etc.) */
		
		
		// == Functions.


		// == Cycle functions.
		/** Fetches the next opcode and increments the program counter. */
		void								FetchOpcodeAndIncPc();							// Cycle 1 (always).
		/** Reads the next instruction byte and throws it away. */
		void								ReadNextInstByteAndDiscard();					// Cycle 2.
		/** Reads the next instruction byte and throws it away. */
		void								ReadNextInstByteAndDiscardAndIncPc();			// Cycle 2.
		/** Reads the next instruction byte and throws it away, increasing PC. */
		void								NopAndIncPcAndFinish();
		/** Fetches a value using immediate addressing. */
		void								FetchValueAndIncPc_Imm();						// Cycle 2.
		/** Fetches a pointer and increments PC .*/
		void								FetchPointerAndIncPc();							// Cycle 2.
		/** Fetches an 8-bit address for Zero-Page dereferencing and increments PC. Stores the address in LSN_CPU_CONTEXT::a.ui16Address. */
		void								FetchAddressAndIncPc_Zp();						// Cycle 2.
		/** Fetches the low address and writes the value to the low byte of LSN_CPU_CONTEXT::a.ui16Address. */
		void								FetchLowAddrByteAndIncPc_WriteImm();			// Cycle 2.
		/** Fetches the low address value for absolute addressing but does not write the value to LSN_CPU_CONTEXT::a.ui16Address yet.  Pair with FetchHighAddrByteAndIncPc(). */
		void								FetchLowAddrByteAndIncPc();						// Cycle 2.
		/** Fetches the high address value for absolute/indirect addressing. */
		void								FetchHighAddrByteAndIncPc();					// Cycle 3.
		/** Fetches the high address value for absolute/indirect addressing.  Adds Y to the low byte of the resulting address. */
		void								FetchHighAddrByteAndIncPcAndAddY();				// Cycle 3.
		/** Fetches the high address value for absolute/indirect addressing.  Adds X to the low byte of the resulting address. */
		void								FetchHighAddrByteAndIncPcAndAddX();				// Cycle 3.
		/** Reads from the effective address.  The address is in LSN_CPU_CONTEXT::a.ui16Address.  The result is stored in LSN_CPU_CONTEXT::ui8Operand. */
		void								ReadFromEffectiveAddress_Abs();					// Cycle 4.
		/** Reads from the effective address.  The address is in LSN_CPU_CONTEXT::a.ui16Address.  The result is stored in LSN_CPU_CONTEXT::ui8Operand. */
		void								ReadFromEffectiveAddress_Zp();					// Cycles 2, 4.
		/** Reads from LSN_CPU_CONTEXT::ui8Pointer, stores the result into LSN_CPU_CONTEXT::ui8Pointer.  Preceded by FetchPointerAndIncPc(). */
		void								ReadAtPointerAddress();							// Cycle 3.
		/** Reads from LSN_CPU_CONTEXT::ui8Pointer, stores (LSN_CPU_CONTEXT::ui8Pointer+X)&0xFF into LSN_CPU_CONTEXT::a.ui16Address.  Preceded by FetchPointerAndIncPc(). */
		void								ReadFromAddressAndAddX_ZpX();
		/** Reads from LSN_CPU_CONTEXT::ui8Pointer, stores (LSN_CPU_CONTEXT::ui8Pointer+Y)&0xFF into LSN_CPU_CONTEXT::a.ui16Address.  Preceded by FetchPointerAndIncPc(). */
		void								ReadFromAddressAndAddX_ZpY();
		/** Reads from LSN_CPU_CONTEXT::a.ui16Address+Y, stores the result into LSN_CPU_CONTEXT::ui8Operand. */
		void								AddYAndReadAddress_IndY();						// Cycle 5.
		/** 4th cycle of JMP (indorect). */
		void								Jmp_Ind_Cycle4();								// Cycle 4.
		/** 3rd cycle of JSR. */
		void								Jsr_Cycle3();									// Cycle 3.
		/** 4th cycle of JSR. */
		void								Jsr_Cycle4();									// Cycle 4.
		/** 5th cycle of JSR. */
		void								Jsr_Cycle5();									// Cycle 5.
		/** 3rd cycle of PLA/PLP/RTI/RTS. */
		void								PLA_PLP_RTI_RTS_Cycle3();						// Cycle 3.
		/** Pushes PCH. */
		void								PushPch();										// Cycle 3.
		/** Pushes PCH, sets the B flag, and decrements S. */
		//void								PushPchWithBFlag();								// Cycle 3.
		/** Pushes PCL, decrements S. */
		void								PushPcl();										// Cycle 4.
		/** Pushes status with B. */
		void								PushStatusAndBAndSetAddressByIrq();				// Cycle 5.
		/** Pushes status without B. */
		void								PushStatusAndNoBAndSetAddressByIrq();			// Cycle 5.
		/** Pushes status an decrements S. */
		void								PushStatus();									// Cycle 5.
		/** Pushes status without B. */
		//void								PushStatusAndSetAddressByIrq();					// Cycle 5.
		/** Pulls status, ignoring B. */
		void								PullStatusWithoutB();							// Cycle 4.
		/** Pulls status. */
		void								PullStatus();									// Cycle 4.
		/** Pulls PCL. */
		void								PullPcl();										// Cycle 5.
		/** Pulls PCH. */
		void								PullPch();										// Cycle 6.
		/** Reads from LSN_CPU_CONTEXT::ui8Pointer, adds X to it, stores the result in LSN_CPU_CONTEXT::ui8Pointer. */
		void								ReadAddressAddX_IzX();							// Cycle 3.
		/** Reads the low byte of the effective address using LSN_CPU_CONTEXT::ui8Pointer+X, store in the low byte of LSN_CPU_CONTEXT::a.ui16Address. */
		void								FetchEffectiveAddressLow_IzX();					// Cycle 4.
		/** Reads the high byte of the effective address using LSN_CPU_CONTEXT::ui8Pointer+X+1, store in the high byte of LSN_CPU_CONTEXT::a.ui16Address. */
		void								FetchEffectiveAddressHigh_IzX();				// Cycle 5.
		/** Reads the low byte of the effective address using LSN_CPU_CONTEXT::ui8Pointer, store in the low byte of LSN_CPU_CONTEXT::a.ui16Address. */
		void								FetchEffectiveAddressLow_IzY();					// Cycle 3.
		/** Reads the high byte of the effective address using LSN_CPU_CONTEXT::ui8Pointer+1, store in the high byte of LSN_CPU_CONTEXT::a.ui16Address, adds Y. */
		void								FetchEffectiveAddressHigh_IzY();				// Cycle 4.
		/** Reads from the effective address (LSN_CPU_CONTEXT::a.ui16Address), which may be wrong if a page boundary was crossed.  If so, fixes the high byte of LSN_CPU_CONTEXT::a.ui16Address. */
		template <bool _bHandleCrossing>
		void								ReadEffectiveAddressFixHighByte_IzX_IzY_AbX();		// Cycle 5.
		/** Fetches the low byte of the NMI/IRQ/BRK/reset vector (stored in LSN_CPU_CONTEXT::a.ui16Address) into the low byte of PC and sets the I flag. */
		void								CopyVectorPcl();								// Cycle 6.
		/** Fetches the high byte of the NMI/IRQ/BRK/reset vector (stored in LSN_CPU_CONTEXT::a.ui16Address) into the high byte of PC. */
		void								CopyVectorPch();								// Cycle 7.
		/** Fetches the low byte of PC from $FFFE. */
		void								FetchPclFromFFFE();								// Cycle 6.
		/** Fetches the high byte of PC from $FFFF. */
		void								FetchPclFromFFFF();								// Cycle 7.
		/** Writes the operand value back to the effective address stored in LSN_CPU_CONTEXT::a.ui16Address. */
		void								WriteValueBackToEffectiveAddress();				// Cycle 5.
		/** Writes the operand value back to the effective address stored in LSN_CPU_CONTEXT::a.ui16Address&0xFF. */
		void								WriteValueBackToEffectiveAddress_Zp();			// Cycle 5.
		/** 2nd cycle of branch instructions. Reads the operand (JMP offset). */
		void								Branch_Cycle2();
		/** 3rd cycle of branch instructions. Fetches opcode of next instruction and performs the check to decide which cycle comes next (or to end the instruction). */
		template <unsigned _uBit, unsigned _uVal>
		void								Branch_Cycle3();
		/** 2nd cycle of branch instructions. Performs the check to decide which cycle comes next (or to end the instruction). */
		/*template <unsigned _uBit, unsigned _uVal>
		void								Branch_Cycle2();*/
		/** 3rd cycle of branch instructions. Branch was taken, so copy part of the address and check for a page boundary. */
		//void								Branch_Cycle3();
		/** 4th cycle of branch instructions. Branch was taken and crossed a page boundary, but PC is already up-to-date so read/discard/exit. */
		void								Branch_Cycle4();
		/** 5th cycle of branch instructions. Page boundary was crossed. */
		void								Branch_Cycle5();
		/** Performs m_pbBus->Write( m_pccCurContext->a.ui16Address, m_pccCurContext->ui8Operand ); and LSN_FINISH_INST;, which finishes Read-Modify-Write instructions. */
		void								FinalWriteCycle();
		/**
		 * Performs a compare against a register and an operand by setting flags.
		 *
		 * \param _ui8RegVal The register value used in the comparison.
		 * \param _ui8OpVal The operand value used in the comparison.
		 */
		inline void							Cmp( uint8_t _ui8RegVal, uint8_t _ui8OpVal );
		/**
		 * Performs an add-with-carry with an operand, setting flags X, N, and Z.
		 *
		 * \param _ui8RegVal The register value used in the comparison.
		 * \param _ui8OpVal The operand value used in the comparison.
		 */
		inline void							Adc( uint8_t _ui8RegVal, uint8_t _ui8OpVal );

		/**
		 * Prepares to enter a new instruction.
		 *
		 * \param _ui16Op The instruction to begin executing.
		 */
		inline void							BeginInst( uint16_t _ui16Op );

		// == Work functions.
		/** Performs A += OP + C.  Sets flags C, V, N and Z. */
		void								ADC_IzX_IzY_ZpX_AbX_AbY_Zp_Abs();
		/** Performs A += OP + C.  Sets flags C, V, N and Z. */
		void								ADC_IzY_AbX_AbY_1();
		/** Performs A += OP + C.  Sets flags C, V, N and Z. */
		void								ADC_Imm();
		/** Fetches from PC and performs A = A & OP.  Sets flags N and Z. */
		void								ANC_Imm();
		/** Fetches from LSN_CPU_CONTEXT::a.ui16Address and performs A = A & OP.  Sets flags N and Z. */
		void								AND_IzX_IzY_ZpX_AbX_AbY_Zp_Abs();
		/** Fetches from LSN_CPU_CONTEXT::a.ui16Address and performs A = A & OP.  Sets flags N and Z. */
		void								AND_IzY_AbX_AbY_1();
		/** Fetches from PC and performs A = A & OP.  Sets flags N and Z. */
		void								AND_Imm();
		/** Fetches from PC and performs A = (A | CONST) & X & OP.  Sets flags N and Z. */
		void								ANE();
		/** Fetches from PC and performs A = A & OP; A = (A >> 1) | (C << 7).  Sets flags C, V, N and Z. */
		void								ARR_Imm();
		/** Performs OP <<= 1.  Sets flags C, N, and Z. */
		void								ASL_IzX_IzY_ZpX_AbX_AbY_Zp_Abs();
		/** Performs A <<= 1.  Sets flags C, N, and Z. */
		void								ASL_Imp();
		/** Performs A &= OP; A >>= 1.  Sets flags C, N, and Z. */
		void								ASR_Imm();
		/** Sets flags N, V and Z according to a bit test. */
		void								BIT_IzX_IzY_ZpX_AbX_AbY_Zp_Abs();
		/** Pops the high byte of the NMI/IRQ/BRK/reset vector (stored in LSN_CPU_CONTEXT::a.ui16Address) into the high byte of PC. */
		void								BRK();
		/** Clears the carry flag. */
		void								CLC();
		/** Clears the decimal flag. */
		void								CLD();
		/** Clears the IRQ flag. */
		void								CLI();
		/** Clears the overflow flag. */
		void								CLV();
		/** Compares A with OP. */
		void								CMP_IzX_IzY_ZpX_AbX_AbY_Zp_Abs();
		/** Compares A with OP. */
		void								CMP_IzY_AbX_AbY_1();
		/** Compares A with OP. */
		void								CMP_Imm();
		/** Compares X with OP. */
		void								CPX_IzX_IzY_ZpX_AbX_AbY_Zp_Abs();
		/** Compares X with OP. */
		void								CPX_Imm();
		/** Compares Y with OP. */
		void								CPY_IzX_IzY_ZpX_AbX_AbY_Zp_Abs();
		/** Compares Y with OP. */
		void								CPY_Imm();
		/** Performs [ADDR]--; CMP(A).  Sets flags C, N, and Z. */
		void								DCP_IzX_IzY_ZpX_AbX_AbY_Zp_Abs();
		/** Performs [ADDR]--.  Sets flags N and Z. */
		void								DEC_IzX_IzY_ZpX_AbX_AbY_Zp_Abs();
		/** Performs X--.  Sets flags N and Z. */
		void								DEX();
		/** Performs Y--.  Sets flags N and Z. */
		void								DEY();
		/** Fetches from LSN_CPU_CONTEXT::a.ui16Address and performs A = A ^ OP.  Sets flags N and Z. */
		void								EOR_IzX_IzY_ZpX_AbX_AbY_Zp_Abs();
		/** Fetches from LSN_CPU_CONTEXT::a.ui16Address and performs A = A ^ OP.  Sets flags N and Z. */
		void								EOR_IzY_AbX_AbY_1();
		/** Fetches from PC and performs A = A ^ OP.  Sets flags N and Z. */
		void								EOR_Imm();
		/** Performs [ADDR]++.  Sets flags N and Z. */
		void								INC_IzX_IzY_ZpX_AbX_AbY_Zp_Abs();
		/** Performs X++.  Sets flags N and Z. */
		void								INX();
		/** Performs Y++.  Sets flags N and Z. */
		void								INY();
		/** Performs M++; SBC.  Sets flags C, N, V, and Z. */
		void								ISB_IzX_IzY_ZpX_AbX_AbY_Zp_Abs();
		/** Jams the machine, putting 0xFF on the bus repeatedly. */
		void								JAM();
		/** Copies the read value into the low byte of PC after fetching the high byte. */
		void								JMP_Abs();
		/** Copies the read value into the low byte of PC after fetching the high byte. */
		void								JMP_Ind();
		/** JSR (Jump to Sub-Routine). */
		void								JSR();
		/** Performs A = X = S = (OP & S).  Sets flags N and Z. */
		void								LAS_IzX_IzY_ZpX_AbX_AbY_Zp_Abs();
		/** Performs A = X = S = (OP & S).  Sets flags N and Z. */
		void								LAS_IzY_AbX_AbY_1();
		/** Performs A = X = OP.  Sets flags N and Z. */
		void								LAX_IzX_IzY_ZpX_AbX_AbY_Zp_Abs();
		/** Performs A = X = OP.  Sets flags N and Z. */
		void								LAX_IzY_AbX_AbY_1();
		/** Performs A = OP.  Sets flags N and Z. */
		void								LDA_Imm();
		/** Performs A = OP.  Sets flags N and Z. */
		void								LDA_IzX_IzY_ZpX_AbX_AbY_Zp_Abs();
		/** Performs A = OP.  Sets flags N and Z. */
		void								LDA_IzY_AbX_AbY_1();
		/** Performs X = OP.  Sets flags N and Z. */
		void								LDX_IzX_IzY_ZpX_AbX_AbY_Zp_Abs();
		/** Performs X = OP.  Sets flags N and Z. */
		void								LDX_IzY_AbX_AbY_1();
		/** Performs X = OP.  Sets flags N and Z. */
		void								LDX_Imm();
		/** Performs Y = OP.  Sets flags N and Z. */
		void								LDY_IzX_IzY_ZpX_AbX_AbY_Zp_Abs();
		/** Performs Y = OP.  Sets flags N and Z. */
		void								LDY_IzY_AbX_AbY_1();
		/** Performs Y = OP.  Sets flags N and Z. */
		void								LDY_Imm();
		/** Performs OP >>= 1.  Sets flags C, N, and Z. */
		void								LSR_IzX_IzY_ZpX_AbX_AbY_Zp_Abs();
		/** Performs A >>= 1.  Sets flags C, N, and Z. */
		void								LSR_Imp();
		/** Fetches from PC and performs A = X = (A | CONST) & OP.  Sets flags N and Z. */
		void								LXA();
		/** Reads the next instruction byte and throws it away. */
		void								NOP_Imp();
		/** Reads the next instruction byte and throws it away, increments PC. */
		void								NOP_Imm();
		/** Reads LSN_CPU_CONTEXT::a.ui16Address and throws it away. */
		void								NOP_IzX_IzY_ZpX_AbX_AbY_Zp_Abs();
		/** No operation. */
		void								NOP_IzY_AbX_AbY_1();
		/** Fetches from PC and performs A = A | OP.  Sets flags N and Z. */
		void								ORA_Imm();
		/** Fetches from LSN_CPU_CONTEXT::a.ui16Address and performs A = A | OP.  Sets flags N and Z. */
		void								ORA_IzX_IzY_ZpX_AbX_AbY_Zp_Abs();
		/** Fetches from LSN_CPU_CONTEXT::a.ui16Address and performs A = A | OP.  Sets flags N and Z. */
		void								ORA_IzY_AbX_AbY_1();
		/** Pushes the accumulator. */
		void								PHA();
		/** Pushes the status byte. */
		void								PHP();
		/** Pulls the accumulator. */
		void								PLA();
		/** Pulls the status byte. */
		void								PLP();
		/** Performs OP = (OP << 1) | (C); A = A | (OP).  Sets flags C, N and Z. */
		void								RLA_IzX_IzY_ZpX_AbX_AbY_Zp_Abs();
		/** Performs A = (A << 1) | (C).  Sets flags C, N, and Z. */
		void								ROL_IzX_IzY_ZpX_AbX_AbY_Zp_Abs();
		/** Performs A = (A << 1) | (C).  Sets flags C, N, and Z. */
		void								ROL_Imp();
		/** Performs A = (A >> 1) | (C << 7).  Sets flags C, N, and Z. */
		void								ROR_IzX_IzY_ZpX_AbX_AbY_Zp_Abs();
		/** Performs A = (A >> 1) | (C << 7).  Sets flags C, N, and Z. */
		void								ROR_Imp();
		/** Performs OP = (OP >> 1) | (C << 7); A += OP + C.  Sets flags C, V, N and Z. */
		void								RRA_IzX_IzY_ZpX_AbX_AbY_Zp_Abs();
		/** Pops into PCH. */
		void								RTI();
		/** Reads PC and increments it. */
		void								RTS();
		/** Writes (A & X) to LSN_CPU_CONTEXT::a.ui16Address. */
		void								SAX_IzX_IzY_ZpX_AbX_AbY_Zp_Abs();
		/** Performs A = A - OP + C.  Sets flags C, V, N and Z. */
		void								SBC_IzX_IzY_ZpX_AbX_AbY_Zp_Abs();
		/** Performs A = A - OP + C.  Sets flags C, V, N and Z. */
		void								SBC_IzY_AbX_AbY_1();
		/** Performs A = A - OP + C.  Sets flags C, V, N and Z. */
		void								SBC_Imm();
		/** Fetches from PC and performs X = (A & X) - OP.  Sets flags C, N and Z. */
		void								SBX();
		/** Sets the carry flag. */
		void								SEC();
		/** Sets the decimal flag. */
		void								SED();
		/** Sets the IRQ flag. */
		void								SEI();
		/** Illegal. Stores A & X & (high-byte of address + 1) at the address. */
		void								SHA_IzX_IzY_ZpX_AbX_AbY_Zp_Abs();
		/** Illegal. Puts A & X into SP; stores A & X & (high-byte of address + 1) at the address. */
		void								SHS();
		/** Illegal. Stores X & (high-byte of address + 1) at the address. */
		void								SHX();
		/** Illegal. Stores Y & (high-byte of address + 1) at the address. */
		void								SHY();
		/** Performs OP = (OP << 1); A = A | (OP).  Sets flags C, N and Z. */
		void								SLO_IzX_IzY_ZpX_AbX_AbY_Zp_Abs();
		/** Performs OP = (OP >> 1); A = A ^ (OP).  Sets flags C, N and Z. */
		void								SRE_IzX_IzY_ZpX_AbX_AbY_Zp_Abs();
		/** Writes A to LSN_CPU_CONTEXT::a.ui16Address. */
		void								STA_IzX_IzY_ZpX_AbX_AbY_Zp_Abs();
		/** Writes X to LSN_CPU_CONTEXT::a.ui16Address. */
		void								STX_IzX_IzY_ZpX_AbX_AbY_Zp_Abs();
		/** Writes Y to LSN_CPU_CONTEXT::a.ui16Address. */
		void								STY_IzX_IzY_ZpX_AbX_AbY_Zp_Abs();
		/** Copies A into X.  Sets flags N, and Z. */
		void								TAX();
		/** Copies A into Y.  Sets flags N, and Z. */
		void								TAY();
		/** Copies S into X. */
		void								TSX();
		/** Copies X into A.  Sets flags N, and Z. */
		void								TXA();
		/** Copies X into S. */
		void								TXS();
		/** Copies Y into A.  Sets flags N, and Z. */
		void								TYA();
		
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	// DEFINITIONS
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	// == Fuctions.
	/**
	 * Performs a compare against a register and an operand by setting flags.
	 *
	 * \param _ui8RegVal The register value used in the comparison.
	 * \param _ui8OpVal The operand value used in the comparison.
	 */
	inline void CCpu6502::Cmp( uint8_t _ui8RegVal, uint8_t _ui8OpVal ) {
		// If the value in the accumulator is equal or greater than the compared value, the Carry will be set.
		SetBit<uint8_t( LSN_STATUS_FLAGS::LSN_SF_CARRY )>( m_ui8Status, _ui8RegVal >= _ui8OpVal );
		// The equal (Z) and negative (N) flags will be set based on equality or lack thereof�
		SetBit<uint8_t( LSN_STATUS_FLAGS::LSN_SF_ZERO )>( m_ui8Status, _ui8RegVal == _ui8OpVal );
		// �and the sign (i.e. A>=$80) of the accumulator.
		SetBit<uint8_t( LSN_STATUS_FLAGS::LSN_SF_NEGATIVE )>( m_ui8Status, ((_ui8RegVal - _ui8OpVal) & 0x80) != 0 );
	}

	/**
	 * Performs an add-with-carry with an operand, setting flags X, N, and Z.
	 *
	 * \param _ui8RegVal The register value used in the comparison.
	 * \param _ui8OpVal The operand value used in the comparison.
	 */
	inline void CCpu6502::Adc( uint8_t _ui8RegVal, uint8_t _ui8OpVal ) {
		uint16_t ui16Result = uint16_t( _ui8RegVal ) + uint16_t( _ui8OpVal ) + (m_ui8Status & uint8_t( LSN_STATUS_FLAGS::LSN_SF_CARRY ));
		SetBit<uint8_t( LSN_STATUS_FLAGS::LSN_SF_OVERFLOW )>( m_ui8Status, (~(_ui8RegVal ^ _ui8OpVal) & (_ui8RegVal ^ ui16Result) & 0x80) == 0 );
		_ui8RegVal = uint8_t( ui16Result );
		SetBit<uint8_t( LSN_STATUS_FLAGS::LSN_SF_CARRY )>( m_ui8Status, ui16Result > 0xFF );
		SetBit<uint8_t( LSN_STATUS_FLAGS::LSN_SF_ZERO )>( m_ui8Status, ui16Result == 0x00 );
		SetBit<uint8_t( LSN_STATUS_FLAGS::LSN_SF_NEGATIVE )>( m_ui8Status, (_ui8RegVal & 0x80) != 0 );
	}

	/**
	 * Prepares to enter a new instruction.
	 *
	 * \param _ui16Op The instruction to begin executing.
	 */
	inline void CCpu6502::BeginInst( uint16_t _ui16Op ) {
		// Enter normal instruction context.
		LSN_CPU_CONTEXT ccContext;
		//ccContext.cContext = LSN_C_NORMAL;
		ccContext.ui8Cycle = 1;		// Values as they should be after this function exits (this function representing the first cycle in all instructions).
		ccContext.ui8FuncIdx = 0;	// This function is implicit, so index 0 is the function handler for the 2nd cycle following this one.
		m_vContextStack.push_back( ccContext );
		// Store the pointer to the last item on the stack so it doesn't have to be recalculated over and over.
		m_pccCurContext = &m_vContextStack[m_vContextStack.size()-1];

		// Perform the actual work.
		m_pccCurContext->ui16OpCode = _ui16Op;
	}

}	// namespace lsn
