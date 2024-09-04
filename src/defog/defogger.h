#ifndef DEFOGGER_H
#define DEFOGGER_H

#include <thread>
#include <atomic>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/photo.hpp>
#include "iprocessor.h"


/*!
 * \brief Performs defogging on images to enhance visibility.
 * \details The Defogger class is designed to process images and remove fog or haze,
 * improving the clarity and visibility of the content. It uses image processing techniques
 * to enhance the quality of the images by reducing the effects of fog or haze. The class
 * provides methods to apply the defogging process with adjustable parameters.
 */

class Defogger : public IProcessor{
public:
    Defogger(EventDispatcher& dispatcher);
    ~Defogger();

private:
    /*!
    * \brief processEvents
    * \details Handles and processes events. This function overrides the base class implementation to provide specific event handling logic.
    */
    void processEvents() override;

    /*!
    * \brief getAccessibleType
    * \return The type of events that the processor can handle.
    * \details Overrides the base class method to return the specific event type(s) that this derived class can process.
    */
    Event::Type getAccessibleType() override;

    /*!
    * \brief getThreadInfo
    * \return A std::thread object representing the worker thread associated with this processor.
    * \details Overrides the base class method to provide information about the worker thread used by this processor.
    */
    std::thread getThreadInfo() override;

    /*!
    * \brief defog
    * \param pSource
    * \param pOutput
    * \param pRectSize
    * \param pOmega
    * \param pNumt
    * \return
    * \brief Applies a defogging algorithm to the input image using a dark channel prior approach.
    * \details This function uses a dark channel prior to estimate the atmospheric light and transmission map to perform defogging. The parameters
    *          `pRectSize`, `pOmega`, and `pNumt` are used to adjust the size of the window for dark channel estimation, the atmospheric light
    *          parameter, and the transmission map value, respectively. The defogging process aims to enhance the visibility of the image by reducing
    *          the effects of fog or haze.
    * \note The choice of `pRectSize` influences the effectiveness of the defogging. Larger values may produce smoother results but could also
    *       reduce the visibility of fine details. The `pOmega` value controls the estimation of atmospheric light and affects the overall contrast.
    *       Adjust `pNumt` to fine-tune the transmission map estimation for different levels of fog density.
    */
    void defog(cv::Mat pSource, cv::Mat& pOutput, int pRectSize = 15, double pOmega = 0.95, double pNumt = 0.1);

    /*!
     * \brief darkChannel
     * \param pSource
     * \param pSize
     * \return
     * Find a dark channel
     * \note: The size of the window is a key parameter for the result. The larger the window, the greater the probability of containing dark channels,
     *  the darker the dark channels, and the less obvious the effect of defogging. The general window size is 11-51 Between, that is,
     *  the radius is between 5-25.
     */
    cv::Mat darkChannel(cv::Mat pSource, int pSize);

    /*!
     * \brief atmLight
     * \param pSource
     * \param pDark
     * \return
     * \note: Find the global atmospheric light value A
     */
    void atmLight(cv::Mat pSource, cv::Mat pDark, float pOutA[3]);

    /*!
     * \brief transmissionEsticv::Mate
     * \param pSource
     * \param pOutA
     * \param pSize
     * \param pOmega
     * \return
     * \note: Calculate and calculate the estimated value of transmittance
     * The omega in has obvious meaning, the smaller the value, the less obvious the defogging effect
     */
    cv::Mat transmissionEstimate(cv::Mat pSource, float pOutA[3], int pSize, float pOmega);

    /*!
     * \brief guidedFilter
     * \param pSource
     * \param pTransmissionEsticv::Mate
     * \param pR
     * \param pEps
     * \return
     * \note:Guided filtering
     */
    cv::Mat guidedfilter(cv::Mat pSource, cv::Mat pTransmissionEstimated, int pR, float pEps);

    /*!
     * \brief transmissionRefine
     * \param pSource
     * \param pTransmissionEsticv::Mate
     * \return
     * \note:Calculation of transmittance by guided filtering
     */
    cv::Mat transmissionRefine(cv::Mat pSource, cv::Mat pTransmissionEstimated);

    /*!
     * \brief recover
     * \param pSource
     * \param pTransmission
     * \param pOutA
     * \param pTx
     * \return
     * \note: Image defogging
     */
    cv::Mat recover(cv::Mat pSource, cv::Mat pTransmissionRefined, float pOutA[3], float pTx);

private:
     /*!
     * \brief Returns the indices that would sort a given vector in ascending order.
     * \param pArray A vector of values that needs to be sorted. This vector's elements are used to determine the sorting order.
     * \return A vector of indices representing the order in which the elements of `pArray` would be sorted.
     * \details This template function generates a vector of indices that, when used to access the elements of `pArray`, will return the elements in ascending order. It is useful for obtaining the order of elements without modifying the original vector.
     * \tparam T The type of elements in the input vector. The function works with any data type that supports comparison operators.
     */
    template<typename T>
    std::vector<int> argsort(const std::vector<T>& pArray)
    {
        const int tArrayLen(pArray.size());
        std::vector<int> tArrayIndex(tArrayLen, 0);
        for (int i = 0; i < tArrayLen; ++i)
            tArrayIndex[i] = i;

        sort(tArrayIndex.begin(), tArrayIndex.end(), [&pArray](int tPos1, int tPos2) {
            return (pArray[tPos1] < pArray[tPos2]);
        });

        return tArrayIndex;
    }
};

#endif // DEFOGGER_H
