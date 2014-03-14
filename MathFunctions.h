/**
 * @file: MathFunctions.h
 * @brief: Various functions mostly related to vectors and matrices.
 *
 * @author: Jordi Munoz and William Premerlani
 * @modifications: Connor Martin.
 * @date: Feb 1, 2014
 *
 * @preconditions:
 * @device:
 *
 * @remarks:
 *
 */

#ifndef MATHFUNCTIONS_H
#define	MATHFUNCTIONS_H

#ifdef	__cplusplus
extern "C" {
#endif
    

//******************************************************************************
//Device #defines
//******************************************************************************
#define ToRad(x) (x*0.01745329252)  // *pi/180
#define ToDeg(x) (x*57.2957795131)  // *180/pi


//******************************************************************************
//Function Prototypes
//******************************************************************************

/**
 * Computes the dot product of two vectors.
 *
 * @param vector1: The first vector.
 * @param vector2: The second vector.
 * @return The result of the dot product of the two vectors.
 */
float Vector_Dot_Product(float vector1[3],float vector2[3]);

/**
 * Computes the cross product of two vectors.
 *
 * @param vectorOut: The result of the cross product.
 * @param v1: The first vector.
 * @param v2: The second vector.
 */
void Vector_Cross_Product(float vectorOut[3], float v1[3],float v2[3]);

/**
 * Multiply a vector by a scalar.
 *
 * @param vectorOut: The result of the multiplication.
 * @param vectorIn: The vector.
 * @param scale2: The scalar.
 */
void Vector_Scale(float vectorOut[3],float vectorIn[3], float scale2);

/**
 * Adds two vectors.
 *
 * @param vectorOut: The result of the addition.
 * @param vectorIn1: The first vector.
 * @param vectorIn2: The second vector.
 */
void Vector_Add(float vectorOut[3],float vectorIn1[3], float vectorIn2[3]);

/**
 * Multiply two 3x3 matrixs. This function developed by Jordi can be easily
 * adapted to multiple n*n matrix's. (Pero me da flojera!).
 *
 * @param matrixOut: The resulting multiplication.
 * @param matrixIn1: The first matrix.
 * @param mmatrixIn2: The second matrix.
 */
void Matrix_Multiply(float matrixOut[3][3], float matrixIn1[3][3], float matrixIn2[3][3]);

/**
 * Returns the number constrained to within the minimum and maximum.
 *
 * @param value: The value to be contrained.
 * @param minimum: The minimum the value can be.
 * @param maximum: The maximum the value can be.
 * @return: The contraint value.
 */
float constrain(float value, float minimum, float maximum);

#ifdef	__cplusplus
}
#endif

#endif	/* MATHFUNCTIONS_H */

