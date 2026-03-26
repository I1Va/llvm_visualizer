; ModuleID = './c_examples/front.cpp'
source_filename = "./c_examples/front.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.parsing_block_t = type { ptr, i64, ptr, i64, ptr, i64, ptr, i64, i64, %struct.parser_err_t, ptr, ptr }
%struct.parser_err_t = type { i8, %struct.lexem_t, [128 x i32], i64 }
%struct.lexem_t = type { i32, %union.token_value_t, %struct.text_pos_t, i64, i8 }
%union.token_value_t = type { i64 }
%struct.text_pos_t = type { i64, i64 }

@.str = private unnamed_addr constant [5 x i8] c"data\00", align 1
@.str.1 = private unnamed_addr constant [23 x i8] c"./src/lang_grammar.cpp\00", align 1
@__PRETTY_FUNCTION__._Z9try_gruleP15parsing_block_tPFP15ast_tree_elem_tS0_E = private unnamed_addr constant [87 x i8] c"ast_tree_elem_t *try_grule(parsing_block_t *, ast_tree_elem_t *(*)(parsing_block_t *))\00", align 1
@.str.2 = private unnamed_addr constant [11 x i8] c"grule_func\00", align 1
@__PRETTY_FUNCTION__._Z16start_parser_errP15parsing_block_t7lexem_t16grammar_rule_num = private unnamed_addr constant [73 x i8] c"void start_parser_err(parsing_block_t *, lexem_t, enum grammar_rule_num)\00", align 1
@stdout = external global ptr, align 8
@.str.3 = private unnamed_addr constant [28 x i8] c"!data->parser_err.err_state\00", align 1
@.str.4 = private unnamed_addr constant [47 x i8] c"data->parser_err.lex.token_type == TOKEN_EMPTY\00", align 1
@.str.5 = private unnamed_addr constant [34 x i8] c"!data->parser_err.grule_list_size\00", align 1
@.str.6 = private unnamed_addr constant [11 x i8] c"parser_err\00", align 1
@__PRETTY_FUNCTION__._Z23add_grule_to_parser_errP12parser_err_t16grammar_rule_num = private unnamed_addr constant [68 x i8] c"void add_grule_to_parser_err(parser_err_t *, enum grammar_rule_num)\00", align 1
@.str.7 = private unnamed_addr constant [22 x i8] c"parser_err->err_state\00", align 1
@__PRETTY_FUNCTION__._Z16clear_parser_errP12parser_err_t = private unnamed_addr constant [38 x i8] c"void clear_parser_err(parser_err_t *)\00", align 1
@.str.8 = private unnamed_addr constant [7 x i8] c"stream\00", align 1
@__PRETTY_FUNCTION__._Z16check_parser_errP8_IO_FILEP15parsing_block_t = private unnamed_addr constant [49 x i8] c"bool check_parser_err(FILE *, parsing_block_t *)\00", align 1
@.str.9 = private unnamed_addr constant [13 x i8] c"SYNTAX_ERROR\00", align 1
@.str.10 = private unnamed_addr constant [14 x i8] c"ERROR_LEXEM: \00", align 1
@.str.11 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@.str.12 = private unnamed_addr constant [14 x i8] c"ERROR_PATH: \0A\00", align 1
@__PRETTY_FUNCTION__._Z13dump_last_lexP15parsing_block_t = private unnamed_addr constant [38 x i8] c"void dump_last_lex(parsing_block_t *)\00", align 1
@.str.13 = private unnamed_addr constant [10 x i8] c"DEBUG_LEX\00", align 1
@dump_dynamic_logger_info_path = private unnamed_addr constant [22 x i8] c"info/dynamic_info.bin\00", align 1
@llvm.global_dtors = appending global [1 x { i32, ptr, ptr }] [{ i32, ptr, ptr } { i32 0, ptr @__dtor_wrapper, ptr null }]

; Function Attrs: mustprogress noinline optnone sspstrong uwtable
define dso_local noundef ptr @_Z9try_gruleP15parsing_block_tPFP15ast_tree_elem_tS0_E(ptr noundef %0, ptr noundef %1) #0 {
  call void @basic_block_start_logger(i64 2)
  %3 = alloca ptr, align 8
  %4 = alloca ptr, align 8
  %5 = alloca ptr, align 8
  %6 = alloca i64, align 8
  %7 = alloca ptr, align 8
  store ptr %0, ptr %4, align 8
  store ptr %1, ptr %5, align 8
  %8 = load ptr, ptr %4, align 8
  %9 = icmp ne ptr %8, null
  br i1 %9, label %10, label %11

10:                                               ; preds = %2
  call void @basic_block_start_logger(i64 17)
  br label %13

11:                                               ; preds = %2
  call void @basic_block_start_logger(i64 20)
  call void @call_logger(i64 21, i64 75)
  call void @__assert_fail(ptr noundef @.str, ptr noundef @.str.1, i32 noundef 105, ptr noundef @__PRETTY_FUNCTION__._Z9try_gruleP15parsing_block_tPFP15ast_tree_elem_tS0_E) #7
  unreachable

12:                                               ; No predecessors!
  call void @basic_block_start_logger(i64 23)
  br label %13

13:                                               ; preds = %12, %10
  call void @basic_block_start_logger(i64 25)
  %14 = load ptr, ptr %5, align 8
  %15 = icmp ne ptr %14, null
  br i1 %15, label %16, label %17

16:                                               ; preds = %13
  call void @basic_block_start_logger(i64 31)
  br label %19

17:                                               ; preds = %13
  call void @basic_block_start_logger(i64 34)
  call void @call_logger(i64 35, i64 75)
  call void @__assert_fail(ptr noundef @.str.2, ptr noundef @.str.1, i32 noundef 106, ptr noundef @__PRETTY_FUNCTION__._Z9try_gruleP15parsing_block_tPFP15ast_tree_elem_tS0_E) #7
  unreachable

18:                                               ; No predecessors!
  call void @basic_block_start_logger(i64 37)
  br label %19

19:                                               ; preds = %18, %16
  call void @basic_block_start_logger(i64 39)
  %20 = load ptr, ptr %4, align 8
  %21 = getelementptr inbounds nuw %struct.parsing_block_t, ptr %20, i32 0, i32 7
  %22 = load i64, ptr %21, align 8
  store i64 %22, ptr %6, align 8
  store ptr null, ptr %7, align 8
  %23 = load ptr, ptr %5, align 8
  %24 = load ptr, ptr %4, align 8
  %25 = call noundef ptr %23(ptr noundef %24)
  store ptr %25, ptr %7, align 8
  %26 = load ptr, ptr %4, align 8
  %27 = getelementptr inbounds nuw %struct.parsing_block_t, ptr %26, i32 0, i32 9
  %28 = getelementptr inbounds nuw %struct.parser_err_t, ptr %27, i32 0, i32 0
  %29 = load i8, ptr %28, align 8
  %30 = trunc i8 %29 to i1
  br i1 %30, label %33, label %31

31:                                               ; preds = %19
  call void @basic_block_start_logger(i64 57)
  %32 = load ptr, ptr %7, align 8
  store ptr %32, ptr %3, align 8
  br label %39

33:                                               ; preds = %19
  call void @basic_block_start_logger(i64 62)
  %34 = load ptr, ptr %4, align 8
  %35 = getelementptr inbounds nuw %struct.parsing_block_t, ptr %34, i32 0, i32 9
  call void @call_logger(i64 65, i64 76)
  call void @_Z16clear_parser_errP12parser_err_t(ptr noundef %35)
  %36 = load i64, ptr %6, align 8
  %37 = load ptr, ptr %4, align 8
  %38 = getelementptr inbounds nuw %struct.parsing_block_t, ptr %37, i32 0, i32 7
  store i64 %36, ptr %38, align 8
  store ptr null, ptr %3, align 8
  br label %39

39:                                               ; preds = %33, %31
  call void @basic_block_start_logger(i64 72)
  %40 = load ptr, ptr %3, align 8
  ret ptr %40
}

; Function Attrs: cold noreturn nounwind
declare void @__assert_fail(ptr noundef, ptr noundef, i32 noundef, ptr noundef) #1

; Function Attrs: mustprogress noinline nounwind optnone sspstrong uwtable
define dso_local void @_Z16clear_parser_errP12parser_err_t(ptr noundef %0) #2 {
  call void @basic_block_start_logger(i64 77)
  %2 = alloca ptr, align 8
  %3 = alloca %struct.lexem_t, align 8
  store ptr %0, ptr %2, align 8
  %4 = load ptr, ptr %2, align 8
  %5 = icmp ne ptr %4, null
  br i1 %5, label %6, label %7

6:                                                ; preds = %1
  call void @basic_block_start_logger(i64 87)
  br label %9

7:                                                ; preds = %1
  call void @basic_block_start_logger(i64 90)
  call void @call_logger(i64 91, i64 75)
  call void @__assert_fail(ptr noundef @.str.6, ptr noundef @.str.1, i32 noundef 144, ptr noundef @__PRETTY_FUNCTION__._Z16clear_parser_errP12parser_err_t) #7
  unreachable

8:                                                ; No predecessors!
  call void @basic_block_start_logger(i64 93)
  br label %9

9:                                                ; preds = %8, %6
  call void @basic_block_start_logger(i64 95)
  %10 = load ptr, ptr %2, align 8
  %11 = getelementptr inbounds nuw %struct.parser_err_t, ptr %10, i32 0, i32 0
  %12 = load i8, ptr %11, align 8
  %13 = trunc i8 %12 to i1
  br i1 %13, label %14, label %15

14:                                               ; preds = %9
  call void @basic_block_start_logger(i64 103)
  br label %17

15:                                               ; preds = %9
  call void @basic_block_start_logger(i64 106)
  call void @call_logger(i64 107, i64 75)
  call void @__assert_fail(ptr noundef @.str.7, ptr noundef @.str.1, i32 noundef 145, ptr noundef @__PRETTY_FUNCTION__._Z16clear_parser_errP12parser_err_t) #7
  unreachable

16:                                               ; No predecessors!
  call void @basic_block_start_logger(i64 109)
  br label %17

17:                                               ; preds = %16, %14
  call void @basic_block_start_logger(i64 111)
  %18 = load ptr, ptr %2, align 8
  %19 = getelementptr inbounds nuw %struct.parser_err_t, ptr %18, i32 0, i32 3
  store i64 0, ptr %19, align 8
  %20 = load ptr, ptr %2, align 8
  %21 = getelementptr inbounds nuw %struct.parser_err_t, ptr %20, i32 0, i32 2
  %22 = getelementptr inbounds [128 x i32], ptr %21, i64 0, i64 0
  store i32 0, ptr %22, align 8
  %23 = load ptr, ptr %2, align 8
  %24 = getelementptr inbounds nuw %struct.parser_err_t, ptr %23, i32 0, i32 0
  store i8 0, ptr %24, align 8
  call void @call_logger(i64 122, i64 407)
  call void @llvm.memset.p0.i64(ptr align 8 %3, i8 0, i64 48, i1 false)
  %25 = getelementptr inbounds nuw %struct.lexem_t, ptr %3, i32 0, i32 0
  %26 = getelementptr inbounds nuw %struct.lexem_t, ptr %3, i32 0, i32 1
  %27 = getelementptr inbounds nuw %struct.lexem_t, ptr %3, i32 0, i32 2
  %28 = getelementptr inbounds nuw %struct.text_pos_t, ptr %27, i32 0, i32 0
  %29 = getelementptr inbounds nuw %struct.text_pos_t, ptr %27, i32 0, i32 1
  %30 = getelementptr inbounds nuw %struct.lexem_t, ptr %3, i32 0, i32 3
  %31 = getelementptr inbounds nuw %struct.lexem_t, ptr %3, i32 0, i32 4
  %32 = load ptr, ptr %2, align 8
  %33 = getelementptr inbounds nuw %struct.parser_err_t, ptr %32, i32 0, i32 1
  call void @call_logger(i64 132, i64 357)
  call void @llvm.memcpy.p0.p0.i64(ptr align 8 %33, ptr align 8 %3, i64 48, i1 false)
  %34 = load ptr, ptr %2, align 8
  %35 = getelementptr inbounds nuw %struct.parser_err_t, ptr %34, i32 0, i32 1
  %36 = getelementptr inbounds nuw %struct.lexem_t, ptr %35, i32 0, i32 0
  store i32 0, ptr %36, align 8
  ret void
}

; Function Attrs: mustprogress noinline optnone sspstrong uwtable
define dso_local void @_Z16start_parser_errP15parsing_block_t7lexem_t16grammar_rule_num(ptr noundef %0, ptr noundef byval(%struct.lexem_t) align 8 %1, i32 noundef %2) #0 {
  call void @basic_block_start_logger(i64 139)
  %4 = alloca ptr, align 8
  %5 = alloca i32, align 4
  store ptr %0, ptr %4, align 8
  store i32 %2, ptr %5, align 4
  %6 = load ptr, ptr %4, align 8
  %7 = icmp ne ptr %6, null
  br i1 %7, label %8, label %9

8:                                                ; preds = %3
  call void @basic_block_start_logger(i64 151)
  br label %11

9:                                                ; preds = %3
  call void @basic_block_start_logger(i64 154)
  call void @call_logger(i64 155, i64 75)
  call void @__assert_fail(ptr noundef @.str, ptr noundef @.str.1, i32 noundef 121, ptr noundef @__PRETTY_FUNCTION__._Z16start_parser_errP15parsing_block_t7lexem_t16grammar_rule_num) #7
  unreachable

10:                                               ; No predecessors!
  call void @basic_block_start_logger(i64 157)
  br label %11

11:                                               ; preds = %10, %8
  call void @basic_block_start_logger(i64 159)
  %12 = load ptr, ptr %4, align 8
  %13 = getelementptr inbounds nuw %struct.parsing_block_t, ptr %12, i32 0, i32 9
  %14 = getelementptr inbounds nuw %struct.parser_err_t, ptr %13, i32 0, i32 0
  %15 = load i8, ptr %14, align 8
  %16 = trunc i8 %15 to i1
  br i1 %16, label %17, label %32

17:                                               ; preds = %11
  call void @basic_block_start_logger(i64 168)
  %18 = load ptr, ptr @stdout, align 8
  %19 = load ptr, ptr %4, align 8
  call void @call_logger(i64 171, i64 250)
  %20 = call noundef zeroext i1 @_Z16check_parser_errP8_IO_FILEP15parsing_block_t(ptr noundef %18, ptr noundef %19)
  %21 = sext i1 %20 to i64
  call void @res_int_logger(i64 %21, i64 171)
  %22 = load ptr, ptr %4, align 8
  %23 = getelementptr inbounds nuw %struct.parsing_block_t, ptr %22, i32 0, i32 9
  %24 = getelementptr inbounds nuw %struct.parser_err_t, ptr %23, i32 0, i32 0
  %25 = load i8, ptr %24, align 8
  %26 = trunc i8 %25 to i1
  %27 = xor i1 %26, true
  br i1 %27, label %28, label %29

28:                                               ; preds = %17
  call void @basic_block_start_logger(i64 181)
  br label %31

29:                                               ; preds = %17
  call void @basic_block_start_logger(i64 184)
  call void @call_logger(i64 185, i64 75)
  call void @__assert_fail(ptr noundef @.str.3, ptr noundef @.str.1, i32 noundef 124, ptr noundef @__PRETTY_FUNCTION__._Z16start_parser_errP15parsing_block_t7lexem_t16grammar_rule_num) #7
  unreachable

30:                                               ; No predecessors!
  call void @basic_block_start_logger(i64 187)
  br label %31

31:                                               ; preds = %30, %28
  call void @basic_block_start_logger(i64 189)
  br label %32

32:                                               ; preds = %31, %11
  call void @basic_block_start_logger(i64 191)
  %33 = load ptr, ptr %4, align 8
  %34 = getelementptr inbounds nuw %struct.parsing_block_t, ptr %33, i32 0, i32 9
  %35 = getelementptr inbounds nuw %struct.parser_err_t, ptr %34, i32 0, i32 1
  %36 = getelementptr inbounds nuw %struct.lexem_t, ptr %35, i32 0, i32 0
  %37 = load i32, ptr %36, align 8
  %38 = icmp eq i32 %37, 0
  br i1 %38, label %39, label %40

39:                                               ; preds = %32
  call void @basic_block_start_logger(i64 201)
  br label %42

40:                                               ; preds = %32
  call void @basic_block_start_logger(i64 204)
  call void @call_logger(i64 205, i64 75)
  call void @__assert_fail(ptr noundef @.str.4, ptr noundef @.str.1, i32 noundef 127, ptr noundef @__PRETTY_FUNCTION__._Z16start_parser_errP15parsing_block_t7lexem_t16grammar_rule_num) #7
  unreachable

41:                                               ; No predecessors!
  call void @basic_block_start_logger(i64 207)
  br label %42

42:                                               ; preds = %41, %39
  call void @basic_block_start_logger(i64 209)
  %43 = load ptr, ptr %4, align 8
  %44 = getelementptr inbounds nuw %struct.parsing_block_t, ptr %43, i32 0, i32 9
  %45 = getelementptr inbounds nuw %struct.parser_err_t, ptr %44, i32 0, i32 3
  %46 = load i64, ptr %45, align 8
  %47 = icmp ne i64 %46, 0
  %48 = xor i1 %47, true
  br i1 %48, label %49, label %50

49:                                               ; preds = %42
  call void @basic_block_start_logger(i64 219)
  br label %52

50:                                               ; preds = %42
  call void @basic_block_start_logger(i64 222)
  call void @call_logger(i64 223, i64 75)
  call void @__assert_fail(ptr noundef @.str.5, ptr noundef @.str.1, i32 noundef 128, ptr noundef @__PRETTY_FUNCTION__._Z16start_parser_errP15parsing_block_t7lexem_t16grammar_rule_num) #7
  unreachable

51:                                               ; No predecessors!
  call void @basic_block_start_logger(i64 225)
  br label %52

52:                                               ; preds = %51, %49
  call void @basic_block_start_logger(i64 227)
  %53 = load ptr, ptr %4, align 8
  %54 = getelementptr inbounds nuw %struct.parsing_block_t, ptr %53, i32 0, i32 9
  %55 = getelementptr inbounds nuw %struct.parser_err_t, ptr %54, i32 0, i32 0
  store i8 1, ptr %55, align 8
  %56 = load ptr, ptr %4, align 8
  %57 = getelementptr inbounds nuw %struct.parsing_block_t, ptr %56, i32 0, i32 9
  %58 = getelementptr inbounds nuw %struct.parser_err_t, ptr %57, i32 0, i32 1
  call void @call_logger(i64 235, i64 357)
  call void @llvm.memcpy.p0.p0.i64(ptr align 8 %58, ptr align 8 %1, i64 48, i1 false)
  %59 = load i32, ptr %5, align 4
  %60 = load ptr, ptr %4, align 8
  %61 = getelementptr inbounds nuw %struct.parsing_block_t, ptr %60, i32 0, i32 9
  %62 = getelementptr inbounds nuw %struct.parser_err_t, ptr %61, i32 0, i32 2
  %63 = getelementptr inbounds [128 x i32], ptr %62, i64 0, i64 0
  store i32 %59, ptr %63, align 8
  %64 = load ptr, ptr %4, align 8
  %65 = getelementptr inbounds nuw %struct.parsing_block_t, ptr %64, i32 0, i32 9
  %66 = getelementptr inbounds nuw %struct.parser_err_t, ptr %65, i32 0, i32 3
  %67 = load i64, ptr %66, align 8
  %68 = add i64 %67, 1
  store i64 %68, ptr %66, align 8
  ret void
}

; Function Attrs: mustprogress noinline optnone sspstrong uwtable
define dso_local noundef zeroext i1 @_Z16check_parser_errP8_IO_FILEP15parsing_block_t(ptr noundef %0, ptr noundef %1) #0 {
  call void @basic_block_start_logger(i64 251)
  %3 = alloca i1, align 1
  %4 = alloca ptr, align 8
  %5 = alloca ptr, align 8
  %6 = alloca %struct.lexem_t, align 8
  %7 = alloca i64, align 8
  store ptr %0, ptr %4, align 8
  store ptr %1, ptr %5, align 8
  %8 = load ptr, ptr %4, align 8
  %9 = icmp ne ptr %8, null
  br i1 %9, label %10, label %11

10:                                               ; preds = %2
  call void @basic_block_start_logger(i64 266)
  br label %13

11:                                               ; preds = %2
  call void @basic_block_start_logger(i64 269)
  call void @call_logger(i64 270, i64 75)
  call void @__assert_fail(ptr noundef @.str.8, ptr noundef @.str.1, i32 noundef 155, ptr noundef @__PRETTY_FUNCTION__._Z16check_parser_errP8_IO_FILEP15parsing_block_t) #7
  unreachable

12:                                               ; No predecessors!
  call void @basic_block_start_logger(i64 272)
  br label %13

13:                                               ; preds = %12, %10
  call void @basic_block_start_logger(i64 274)
  %14 = load ptr, ptr %5, align 8
  %15 = icmp ne ptr %14, null
  br i1 %15, label %16, label %17

16:                                               ; preds = %13
  call void @basic_block_start_logger(i64 280)
  br label %19

17:                                               ; preds = %13
  call void @basic_block_start_logger(i64 283)
  call void @call_logger(i64 284, i64 75)
  call void @__assert_fail(ptr noundef @.str, ptr noundef @.str.1, i32 noundef 156, ptr noundef @__PRETTY_FUNCTION__._Z16check_parser_errP8_IO_FILEP15parsing_block_t) #7
  unreachable

18:                                               ; No predecessors!
  call void @basic_block_start_logger(i64 286)
  br label %19

19:                                               ; preds = %18, %16
  call void @basic_block_start_logger(i64 288)
  %20 = load ptr, ptr %5, align 8
  %21 = getelementptr inbounds nuw %struct.parsing_block_t, ptr %20, i32 0, i32 9
  %22 = getelementptr inbounds nuw %struct.parser_err_t, ptr %21, i32 0, i32 0
  %23 = load i8, ptr %22, align 8
  %24 = trunc i8 %23 to i1
  br i1 %24, label %26, label %25

25:                                               ; preds = %19
  call void @basic_block_start_logger(i64 297)
  store i1 false, ptr %3, align 1
  br label %67

26:                                               ; preds = %19
  call void @basic_block_start_logger(i64 301)
  %27 = load ptr, ptr %4, align 8
  call void @call_logger(i64 303, i64 408)
  call void @_Z13fprintf_titleP8_IO_FILEPKccm(ptr noundef %27, ptr noundef @.str.9, i8 noundef signext 45, i64 noundef 100)
  %28 = load ptr, ptr %4, align 8
  call void @call_logger(i64 305, i64 409)
  %29 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %28, ptr noundef @.str.10) #8
  %30 = sext i32 %29 to i64
  call void @res_int_logger(i64 %30, i64 305)
  %31 = load ptr, ptr %4, align 8
  %32 = load ptr, ptr %5, align 8
  %33 = load ptr, ptr %5, align 8
  %34 = getelementptr inbounds nuw %struct.parsing_block_t, ptr %33, i32 0, i32 9
  %35 = getelementptr inbounds nuw %struct.parser_err_t, ptr %34, i32 0, i32 1
  call void @call_logger(i64 311, i64 357)
  call void @llvm.memcpy.p0.p0.i64(ptr align 8 %6, ptr align 8 %35, i64 48, i1 false)
  call void @call_logger(i64 312, i64 410)
  call void @_Z10lexem_dumpP8_IO_FILEP15parsing_block_t7lexem_t(ptr noundef %31, ptr noundef %32, ptr noundef byval(%struct.lexem_t) align 8 %6)
  call void @call_logger(i64 313, i64 411)
  %36 = call i32 (ptr, ...) @printf(ptr noundef @.str.11)
  %37 = sext i32 %36 to i64
  call void @res_int_logger(i64 %37, i64 313)
  %38 = load ptr, ptr %4, align 8
  call void @call_logger(i64 315, i64 409)
  %39 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %38, ptr noundef @.str.12) #8
  %40 = sext i32 %39 to i64
  call void @res_int_logger(i64 %40, i64 315)
  store i64 0, ptr %7, align 8
  br label %41

41:                                               ; preds = %59, %26
  call void @basic_block_start_logger(i64 319)
  %42 = load i64, ptr %7, align 8
  %43 = load ptr, ptr %5, align 8
  %44 = getelementptr inbounds nuw %struct.parsing_block_t, ptr %43, i32 0, i32 9
  %45 = getelementptr inbounds nuw %struct.parser_err_t, ptr %44, i32 0, i32 3
  %46 = load i64, ptr %45, align 8
  %47 = icmp ult i64 %42, %46
  br i1 %47, label %48, label %62

48:                                               ; preds = %41
  call void @basic_block_start_logger(i64 329)
  %49 = load ptr, ptr %4, align 8
  %50 = load ptr, ptr %5, align 8
  %51 = getelementptr inbounds nuw %struct.parsing_block_t, ptr %50, i32 0, i32 9
  %52 = getelementptr inbounds nuw %struct.parser_err_t, ptr %51, i32 0, i32 2
  %53 = load i64, ptr %7, align 8
  %54 = getelementptr inbounds nuw [128 x i32], ptr %52, i64 0, i64 %53
  %55 = load i32, ptr %54, align 4
  call void @call_logger(i64 337, i64 412)
  call void @_Z10grule_dumpP8_IO_FILE16grammar_rule_num(ptr noundef %49, i32 noundef %55)
  %56 = load ptr, ptr %4, align 8
  call void @call_logger(i64 339, i64 409)
  %57 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %56, ptr noundef @.str.11) #8
  %58 = sext i32 %57 to i64
  call void @res_int_logger(i64 %58, i64 339)
  br label %59

59:                                               ; preds = %48
  call void @basic_block_start_logger(i64 342)
  %60 = load i64, ptr %7, align 8
  %61 = add i64 %60, 1
  store i64 %61, ptr %7, align 8
  br label %41, !llvm.loop !6

62:                                               ; preds = %41
  call void @basic_block_start_logger(i64 347)
  %63 = load ptr, ptr %4, align 8
  call void @call_logger(i64 349, i64 409)
  %64 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %63, ptr noundef @.str.11) #8
  %65 = sext i32 %64 to i64
  call void @res_int_logger(i64 %65, i64 349)
  %66 = load ptr, ptr %4, align 8
  call void @call_logger(i64 351, i64 413)
  call void @_Z14fprintf_borderP8_IO_FILEcmb(ptr noundef %66, i8 noundef signext 45, i64 noundef 100, i1 noundef zeroext true)
  store i1 true, ptr %3, align 1
  br label %67

67:                                               ; preds = %62, %25
  call void @basic_block_start_logger(i64 354)
  %68 = load i1, ptr %3, align 1
  ret i1 %68
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias writeonly captures(none), ptr noalias readonly captures(none), i64, i1 immarg) #3

; Function Attrs: mustprogress noinline nounwind optnone sspstrong uwtable
define dso_local void @_Z23add_grule_to_parser_errP12parser_err_t16grammar_rule_num(ptr noundef %0, i32 noundef %1) #2 {
  call void @basic_block_start_logger(i64 359)
  %3 = alloca ptr, align 8
  %4 = alloca i32, align 4
  store ptr %0, ptr %3, align 8
  store i32 %1, ptr %4, align 4
  %5 = load ptr, ptr %3, align 8
  %6 = icmp ne ptr %5, null
  br i1 %6, label %7, label %8

7:                                                ; preds = %2
  call void @basic_block_start_logger(i64 371)
  br label %10

8:                                                ; preds = %2
  call void @basic_block_start_logger(i64 374)
  call void @call_logger(i64 375, i64 75)
  call void @__assert_fail(ptr noundef @.str.6, ptr noundef @.str.1, i32 noundef 137, ptr noundef @__PRETTY_FUNCTION__._Z23add_grule_to_parser_errP12parser_err_t16grammar_rule_num) #7
  unreachable

9:                                                ; No predecessors!
  call void @basic_block_start_logger(i64 377)
  br label %10

10:                                               ; preds = %9, %7
  call void @basic_block_start_logger(i64 379)
  %11 = load ptr, ptr %3, align 8
  %12 = getelementptr inbounds nuw %struct.parser_err_t, ptr %11, i32 0, i32 0
  %13 = load i8, ptr %12, align 8
  %14 = trunc i8 %13 to i1
  br i1 %14, label %15, label %16

15:                                               ; preds = %10
  call void @basic_block_start_logger(i64 387)
  br label %18

16:                                               ; preds = %10
  call void @basic_block_start_logger(i64 390)
  call void @call_logger(i64 391, i64 75)
  call void @__assert_fail(ptr noundef @.str.7, ptr noundef @.str.1, i32 noundef 138, ptr noundef @__PRETTY_FUNCTION__._Z23add_grule_to_parser_errP12parser_err_t16grammar_rule_num) #7
  unreachable

17:                                               ; No predecessors!
  call void @basic_block_start_logger(i64 393)
  br label %18

18:                                               ; preds = %17, %15
  call void @basic_block_start_logger(i64 395)
  %19 = load i32, ptr %4, align 4
  %20 = load ptr, ptr %3, align 8
  %21 = getelementptr inbounds nuw %struct.parser_err_t, ptr %20, i32 0, i32 2
  %22 = load ptr, ptr %3, align 8
  %23 = getelementptr inbounds nuw %struct.parser_err_t, ptr %22, i32 0, i32 3
  %24 = load i64, ptr %23, align 8
  %25 = add i64 %24, 1
  store i64 %25, ptr %23, align 8
  %26 = getelementptr inbounds nuw [128 x i32], ptr %21, i64 0, i64 %24
  store i32 %19, ptr %26, align 4
  ret void
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr writeonly captures(none), i8, i64, i1 immarg) #4

declare void @_Z13fprintf_titleP8_IO_FILEPKccm(ptr noundef, ptr noundef, i8 noundef signext, i64 noundef) #5

; Function Attrs: nounwind
declare i32 @fprintf(ptr noundef, ptr noundef, ...) #6

declare void @_Z10lexem_dumpP8_IO_FILEP15parsing_block_t7lexem_t(ptr noundef, ptr noundef, ptr noundef byval(%struct.lexem_t) align 8) #5

declare i32 @printf(ptr noundef, ...) #5

declare void @_Z10grule_dumpP8_IO_FILE16grammar_rule_num(ptr noundef, i32 noundef) #5

declare void @_Z14fprintf_borderP8_IO_FILEcmb(ptr noundef, i8 noundef signext, i64 noundef, i1 noundef zeroext) #5

; Function Attrs: mustprogress noinline optnone sspstrong uwtable
define dso_local void @_Z13dump_last_lexP15parsing_block_t(ptr noundef %0) #0 {
  call void @basic_block_start_logger(i64 415)
  %2 = alloca ptr, align 8
  %3 = alloca %struct.lexem_t, align 8
  store ptr %0, ptr %2, align 8
  %4 = load ptr, ptr %2, align 8
  %5 = icmp ne ptr %4, null
  br i1 %5, label %6, label %7

6:                                                ; preds = %1
  call void @basic_block_start_logger(i64 425)
  br label %9

7:                                                ; preds = %1
  call void @basic_block_start_logger(i64 428)
  call void @call_logger(i64 429, i64 75)
  call void @__assert_fail(ptr noundef @.str, ptr noundef @.str.1, i32 noundef 181, ptr noundef @__PRETTY_FUNCTION__._Z13dump_last_lexP15parsing_block_t) #7
  unreachable

8:                                                ; No predecessors!
  call void @basic_block_start_logger(i64 431)
  br label %9

9:                                                ; preds = %8, %6
  call void @basic_block_start_logger(i64 433)
  %10 = load ptr, ptr @stdout, align 8
  call void @call_logger(i64 435, i64 408)
  call void @_Z13fprintf_titleP8_IO_FILEPKccm(ptr noundef %10, ptr noundef @.str.13, i8 noundef signext 45, i64 noundef 100)
  %11 = load ptr, ptr @stdout, align 8
  %12 = load ptr, ptr %2, align 8
  %13 = load ptr, ptr %2, align 8
  %14 = getelementptr inbounds nuw %struct.parsing_block_t, ptr %13, i32 0, i32 6
  %15 = load ptr, ptr %14, align 8
  %16 = load ptr, ptr %2, align 8
  %17 = getelementptr inbounds nuw %struct.parsing_block_t, ptr %16, i32 0, i32 7
  %18 = load i64, ptr %17, align 8
  %19 = getelementptr inbounds nuw %struct.lexem_t, ptr %15, i64 %18
  call void @call_logger(i64 445, i64 357)
  call void @llvm.memcpy.p0.p0.i64(ptr align 8 %3, ptr align 8 %19, i64 48, i1 false)
  call void @call_logger(i64 446, i64 410)
  call void @_Z10lexem_dumpP8_IO_FILEP15parsing_block_t7lexem_t(ptr noundef %11, ptr noundef %12, ptr noundef byval(%struct.lexem_t) align 8 %3)
  call void @call_logger(i64 447, i64 411)
  %20 = call i32 (ptr, ...) @printf(ptr noundef @.str.11)
  %21 = sext i32 %20 to i64
  call void @res_int_logger(i64 %21, i64 447)
  %22 = load ptr, ptr @stdout, align 8
  call void @call_logger(i64 449, i64 413)
  call void @_Z14fprintf_borderP8_IO_FILEcmb(ptr noundef %22, i8 noundef signext 45, i64 noundef 100, i1 noundef zeroext true)
  ret void
}

declare void @call_logger(i64, i64)

declare void @res_int_logger(i64, i64)

declare void @basic_block_start_logger(i64)

define internal void @__dtor_wrapper() {
  call void @dump_dynamic_logger_info(ptr @dump_dynamic_logger_info_path)
  ret void
}

declare void @dump_dynamic_logger_info(ptr)

attributes #0 = { mustprogress noinline optnone sspstrong uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { cold noreturn nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { mustprogress noinline nounwind optnone sspstrong uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
attributes #4 = { nocallback nofree nounwind willreturn memory(argmem: write) }
attributes #5 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #6 = { nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #7 = { cold noreturn nounwind }
attributes #8 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 21.1.6"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
