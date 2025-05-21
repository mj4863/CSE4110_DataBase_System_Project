# CSE4110 데이터베이스 시스템 프로젝트 모음

이 저장소에는 CSE4110 데이터베이스 시스템 과목의 프로젝트 1과 프로젝트 2 구현 자료가 포함되어 있다.

---

## 프로젝트 1: Real-Estate Office DB 설계 및 쿼리 구현

**과목**: CSE4110 - Database System

### 개요

* **목표**: 개념적 설계(E-R 모델), 논리적 설계(Relational Schema), 샘플 데이터 삽입, 주요 SQL 쿼리 구현을 통해 관계형 DB 구축 경험 획득
* **도메인**: 서울 지역 주택 중개 업무

### 요구사항

1. **E-R 모델링**

   * 엔티티: 에이전트, 매도자, 매수자, 매물, 거래 이력 등
   * 관계(Relationship) 정의 및 PK/FK, Cardinality, Optionality 명시
2. **Relational Schema**

   * E-R 모델을 관계 스키마로 변환 및 중복 제거
3. **쿼리 구현** (예시 7가지)

   1. Mapo구 매물 주소 조회 (₩1,000,000,000 \~ ₩1,500,000,000)
   2. 8학군, 침실≥4, 욕실≥2 매물 조회
   3. 2022년 총 판매액 기준 최다 판매 에이전트 조회
   4. 에이전트별 2022년 평균 판매가 및 시장 대기 기간 조회
   5. 유형별 최고가 매물 사진 조회
   6. 매물 판매 기록 등록 쿼리
   7. 신규 에이전트 추가 쿼리
4. **샘플 데이터**

   * zigbang 데이터 기반 임의 생성 (개인 정보 제외)

### 실행 환경 및 제출물

* **DBMS**: MySQL 8.x
* **ERD 도구**: ERwin 또는 Visual Paradigm Online
* **제출물**:

  * E-R 다이어그램 (PNG/ERwin 파일)
  * Relational Schema 다이어그램 (ERwin .erwin)
  * 샘플 DDL/DML 스크립트 (.sql)
  * 쿼리 스크립트 (.sql)
  * 보고서 (\[project1]학번.pdf)

---

## 프로젝트 2: 스키마 정규화 및 C/ODBC 애플리케이션 구현

**과목**: CSE4110 - Database System

### 개요

* **목표**: 논리적 스키마 BCNF 분해, 물리적 스키마 설계, MySQL ODBC 연동 C/C++ 애플리케이션 구현
* **도메인**: 프로젝트 1과 동일한 Real-Estate Office

### 요구사항

1. **BCNF 분해**: 관계 스키마를 BCNF로 변환·검증
2. **물리적 스키마 설계**: ERwin 사용, 데이터 타입/제약조건/NULL 허용 정보 포함
3. **쿼리 구현**: TYPE1\~TYPE7(하위 유형 포함) SQL 스크립트 작성
4. **C/C++ 애플리케이션**:

   * Visual Studio 2019 + MySQL ODBC Driver
   * CRUD 입력 파일(.txt) 처리
   * 메뉴 기반 인터페이스에서 각 TYPE 쿼리 실행 및 결과 출력
   * `quit` 명령 시 종료

### 프로젝트 구조 예시

```
db_project2/
├ schema_logical.png       # 논리 스키마 (BCNF 분해)
├ schema_physical.erwin    # 물리 스키마 ERwin 파일
├ queries/
│  ├ TYPE1.sql
│  └ ...
├ app/
│  ├ main.cpp
│  ├ mysql_odbc_config.txt
│  └ CRUD_queries.txt
└ [project2]학번.pdf       # 보고서
```

### 실행 환경 및 제출물

* **OS**: Windows 10 이상
* **DBMS**: MySQL Server 8.x
* **Development**: Visual Studio 2019
* **ODBC**: MySQL Connector/ODBC 8.x
* **제출물**:

  * 논리 스키마 다이어그램 (PNG)
  * 물리 스키마 ERwin 파일 (.erwin)
  * SQL 스크립트 (queries/\*.sql)
  * C/C++ 소스(app/\*.cpp)
  * 보고서 (\[project2]학번.pdf)

---

## 라이선스

모든 프로젝트에 대해 MIT License 적용
